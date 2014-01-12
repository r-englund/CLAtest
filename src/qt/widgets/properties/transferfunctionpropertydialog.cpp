/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Viktor Axelsson
 *
 **********************************************************************/

#include <inviwo/qt/widgets/properties/transferfunctionpropertydialog.h>

namespace inviwo {

TransferFunctionPropertyDialog::TransferFunctionPropertyDialog(TransferFunctionProperty* tfProperty, QWidget* parent)
    : InviwoDockWidget(tr("Transfer Function"), parent)
    , tfProperty_(tfProperty)
{
    setObjectName("Transfer Function");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    generateWidget();
    updateFromProperty();
}

void TransferFunctionPropertyDialog::generateWidget() {
    vec2 minEditorDims = vec2(255.0f, 100.0f);
    arrayWidth_ = 256; //TODO: set based on data size bit depth
    arrayHeight_ = 256;

    tfEditorView_ = new TransferFunctionEditorView(tfProperty_);
    // put origin to bottom left corner
    tfEditorView_->scale(1.0, -1.0);
    tfEditorView_->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    tfEditorView_->setMinimumSize(minEditorDims.x, minEditorDims.y);
    //tfEditorView_->viewport()->installEventFilter(this);
    //tfEditorView_->setDragMode(QGraphicsView::NoDrag);
    tfEditorView_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tfEditorView_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(tfEditorView_, SIGNAL(resized()), this, SLOT(editorViewResized()));

    tfEditor_ = new TransferFunctionEditor(&tfProperty_->get(), tfEditorView_);
    tfEditor_->setSceneRect(0, 0, minEditorDims.x, minEditorDims.y);
    connect(tfEditor_, SIGNAL(doubleClick()), this, SLOT(showColorDialog()));
    connect(tfEditor_, SIGNAL(selectionChanged()), this, SLOT(updateColorWheel()));
    connect(tfEditor_, SIGNAL(controlPointsChanged()), this, SLOT(updateTransferFunction()));
    tfEditorView_->setScene(tfEditor_);

    zoomVSlider_ = new RangeSliderQt(Qt::Vertical, this);
    zoomVSlider_->setRange(0, 100);
    zoomVSlider_->setValue(static_cast<int>(tfProperty_->getZoomV().x*100), static_cast<int>(tfProperty_->getZoomV().y*100));
    connect(zoomVSlider_, SIGNAL(valuesChanged(int, int)), tfEditorView_, SLOT(zoomVertically(int, int)));

    zoomHSlider_ = new RangeSliderQt(Qt::Horizontal, this);
    zoomHSlider_->setRange(0, 100);
    zoomHSlider_->setValue(static_cast<int>(tfProperty_->getZoomH().x*100), static_cast<int>(tfProperty_->getZoomH().y*100));
    connect(zoomHSlider_, SIGNAL(valuesChanged(int, int)), tfEditorView_, SLOT(zoomHorizontally(int, int)));

    maskSlider_ = new RangeSliderQt(Qt::Horizontal, this);
    int maxMask = arrayWidth_;
    maskSlider_->setRange(0, maxMask);
    maskSlider_->setValue(static_cast<int>(tfProperty_->getMask().x*maxMask), static_cast<int>(tfProperty_->getMask().y*maxMask));
    connect(maskSlider_, SIGNAL(valuesChanged(int, int)), this, SLOT(changeMask(int, int)));

    colorChange_ = false;
    colorWheel_ = new ColorWheel();
    connect(colorWheel_, SIGNAL(colorChange(QColor)), this, SLOT(setPointColor()));

    colorDialog_ = new QColorDialog();
    colorDialog_->setWindowFlags(Qt::WindowStaysOnTopHint);
    connect(colorDialog_, SIGNAL(currentColorChanged(QColor)), this, SLOT(setPointColorDialog()));

    tfPreview_ = new QGraphicsView(this);
    tfPreview_->scale(1.0, -1.0);
    paintScene_ = new QGraphicsScene(this);
    tfPreview_->setScene(paintScene_);
    tfPreview_->setFixedHeight(20.0);
    tfPreview_->setAlignment(Qt::AlignLeft);

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->addWidget(zoomVSlider_,  0, 0);
    gridLayout->addWidget(tfEditorView_, 0, 1);
    gridLayout->addWidget(zoomHSlider_,  1, 1);
    gridLayout->addWidget(tfPreview_,    2, 1);
    gridLayout->addWidget(maskSlider_,   3, 1);
    gridLayout->addWidget(colorWheel_,   0, 2);
    QFrame* frame = new QFrame();
    frame->setLayout(gridLayout);
    setWidget(frame);

    gradient_ = new QLinearGradient(0.0f, 0.0f, tfEditorView_->width(), 0.0f);
    gradientStops_ = new QVector<QGradientStop>();

    updateFromProperty();
}

TransferFunctionPropertyDialog::~TransferFunctionPropertyDialog() {
    delete tfEditor_;
    delete colorWheel_;
    delete gradient_;
    gradientStops_->clear();
    delete gradientStops_;
    delete colorDialog_;
}

void TransferFunctionPropertyDialog::updateTFPreview() {
    int gradientWidth = 255;
    tfPixmap_ = new QPixmap(gradientWidth, 20);
    QPainter tfPainter(tfPixmap_);
    tfPainter.fillRect(0, 0, tfEditorView_->width(), 20, QBrush(*gradient_));
    if (tfProperty_->getMask().x > 0.0f) {
        tfPainter.fillRect(0, 0, static_cast<int>(tfProperty_->getMask().x*tfEditorView_->width()), 20, QColor(25,25,25,100));
        tfPainter.drawLine(static_cast<int>(tfProperty_->getMask().x*tfEditorView_->width()), 0, static_cast<int>(tfProperty_->getMask().x*tfEditorView_->width()), 20);
    }
    if (tfProperty_->getMask().y < 1.0f) {
        tfPainter.fillRect(static_cast<int>(tfProperty_->getMask().y*tfEditorView_->width()), 0, 
            static_cast<int>(tfEditorView_->width()-(tfProperty_->getMask().y*tfEditorView_->width())), 20, QColor(25,25,25,150));
        tfPainter.drawLine(static_cast<int>(tfProperty_->getMask().y*tfEditorView_->width()), 0, static_cast<int>(tfProperty_->getMask().y*tfEditorView_->width()), 20);
    }
    paintScene_->setBackgroundBrush(QBrush((*tfPixmap_).scaled(tfEditorView_->width(), 20)));
    update();
}

void TransferFunctionPropertyDialog::updateFromProperty() {
    QGradientStop* curGradientStop = new QGradientStop();
    gradientStops_->clear();

    TransferFunctionDataPoint* curPoint;
    const vec4* curColor;
    for (size_t i=0; i<tfProperty_->get().getNumberOfDataPoints(); i++) {
        curPoint = tfProperty_->get().getPoint(i);
        curColor = curPoint->getRgba();
        curGradientStop->first = curPoint->getPos()->x;
        // modify the alpha channel to make the colors better visible
        float displayAlpha = curPoint->getPos()->y*2.0;
        if (displayAlpha > 1.0) displayAlpha = 1.0;
        curGradientStop->second = QColor::fromRgbF(curColor->r, curColor->g, curColor->b, displayAlpha);
        gradientStops_->push_front(*curGradientStop);
    }
    gradient_->setStops(*gradientStops_);

    gradient_->setFinalStop(tfEditorView_->width(), 0.0);
    delete curGradientStop;
    updateTFPreview();
}


bool TransferFunctionPropertyDialog::eventFilter(QObject *object, QEvent *e) {
    return false;
}

void TransferFunctionPropertyDialog::notify() {
    updateFromProperty();
    tfProperty_->propertyModified();
}


void TransferFunctionPropertyDialog::updateColorWheel() {
    QList<QGraphicsItem *> selection = tfEditor_->selectedItems();
    if (selection.size()== 1 && dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(0))) {
        const vec4* pointColor = dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(0))->getPoint()->getRgba();
        colorWheel_->setColor(QColor(pointColor->r*255.0, pointColor->g*255.0, pointColor->b*255.0, pointColor->a*255.0));
        if (!colorChange_)
            colorDialog_->setCurrentColor(QColor(pointColor->r*255.0, pointColor->g*255.0, pointColor->b*255.0, pointColor->a*255.0));
    }
}

void TransferFunctionPropertyDialog::showColorDialog() {
    QList<QGraphicsItem *> selection = tfEditor_->selectedItems();
    if (selection.size()==1 && dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(0)))
        colorDialog_->show();
}


void TransferFunctionPropertyDialog::setPointColor(QColor color) {
    vec3* newRgb;
    QList<QGraphicsItem *> selection = tfEditor_->selectedItems();

    for (int i=0; i< selection.size(); i++) {
        if (dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(i))) {
            newRgb = new vec3(color.redF(),color.greenF(),color.blueF());
            dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(i))->getPoint()->setRgb(newRgb);
        }
    }

    updateFromProperty();
    tfEditorView_->update();
    tfProperty_->get().calcTransferValues();
    tfProperty_->propertyModified();
}

void TransferFunctionPropertyDialog::updateTransferFunction() {
    //tfEditorView_->update();
    tfProperty_->get().calcTransferValues();
    updateFromProperty();
    tfProperty_->propertyModified();
    update();
}

void TransferFunctionPropertyDialog::setPointColor() {
    QColor color = colorWheel_->color();
    setPointColor(color);
}

void TransferFunctionPropertyDialog::setPointColorDialog() {
    QColor color = colorDialog_->currentColor();
    setPointColor(color);
    colorChange_ = true;
    updateColorWheel();
    colorChange_ = false;
}


QVector<QGradientStop>* TransferFunctionPropertyDialog::getGradientStops() {
    return gradientStops_;
}

void TransferFunctionPropertyDialog::editorViewResized() {
    gradient_->setStops(*gradientStops_);
    paintScene_->setForegroundBrush(*gradient_);
    gradient_->setFinalStop(tfEditorView_->width(), 0.0);	
}

void TransferFunctionPropertyDialog::changeMask(int maskMin, int maskMax) {
    float maskMinF = static_cast<float>(maskMin)/static_cast<float>(arrayWidth_);
    float maskMaxF = static_cast<float>(maskMax)/static_cast<float>(arrayWidth_);
    tfProperty_->setMask(maskMinF, maskMaxF);
    tfEditorView_->setMask(maskMinF, maskMaxF);
    updateTFPreview();
    tfEditorView_->update();
    tfProperty_->get().calcTransferValues();
    tfProperty_->propertyModified();
}

} // namespace