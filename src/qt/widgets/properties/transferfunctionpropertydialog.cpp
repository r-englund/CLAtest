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

    zoomVSlider_ = new RangeSliderQt(Qt::Vertical, this);
    int maxZoomV = arrayHeight_;
    zoomVSlider_->setRange(0, maxZoomV);
    zoomVSlider_->setValue(static_cast<int>(tfProperty_->getZoomV().x*maxZoomV), static_cast<int>(tfProperty_->getZoomV().y*maxZoomV));
    connect(zoomVSlider_, SIGNAL(valuesChanged(int, int)), this, SLOT(zoomVertically(int, int)));

    zoomHSlider_ = new RangeSliderQt(Qt::Horizontal, this);
    int maxZoomH = arrayWidth_;
    zoomHSlider_->setRange(0, maxZoomH);
    zoomHSlider_->setValue(static_cast<int>(tfProperty_->getZoomH().x*maxZoomH), static_cast<int>(tfProperty_->getZoomH().y*maxZoomH));
    connect(zoomHSlider_, SIGNAL(valuesChanged(int, int)), this, SLOT(zoomHorizontally(int, int)));

    maskSlider_ = new RangeSliderQt(Qt::Horizontal, this);
    int maxMask = arrayWidth_;
    maskSlider_->setRange(0, maxMask);
    maskSlider_->setValue(static_cast<int>(tfProperty_->getMask().x*maxMask), static_cast<int>(tfProperty_->getMask().y*maxMask));
    connect(maskSlider_, SIGNAL(valuesChanged(int, int)), this, SLOT(changeMask(int, int)));

    tfEditorView_ = new TransferFunctionEditorView(tfProperty_);
    tfEditorView_->scale(1.0, -1.0);
    tfEditorView_->setMinimumSize(minEditorDims.x, minEditorDims.y);
    tfEditorView_->viewport()->installEventFilter(this);
    tfEditorView_->setDragMode(QGraphicsView::NoDrag);
    tfEditorView_->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    tfEditorView_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tfEditorView_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(tfEditorView_, SIGNAL(resized()), this, SLOT(editorViewResized()));

    tfEditor_ = new TransferFunctionEditor(&tfProperty_->get(), tfEditorView_);
    addObservation(tfEditor_);
    tfEditor_->addObserver(this);
    tfEditor_->setSceneRect(0, 0, minEditorDims.x, minEditorDims.y);
    ///this->zoomHorizontalChanged();
    connect(tfEditor_, SIGNAL(doubleClick()), this, SLOT(showColorDialog()));
    connect(tfEditor_, SIGNAL(selectionChanged()), this, SLOT(updateColorWheel()));
    tfEditorView_->setScene(tfEditor_);

    colorChange_ = false;
    colorWheel_ = new ColorWheel();
    connect(colorWheel_, SIGNAL(colorChange(QColor)), this, SLOT(setPointColor()));

    colorDialog_ = new QColorDialog();
    colorDialog_->setWindowFlags(Qt::WindowStaysOnTopHint);
    connect(colorDialog_, SIGNAL(currentColorChanged(QColor)), this, SLOT(setPointColorDialog()));

    tfPreview_ = new QGraphicsView(this);
    tfPreview_->scale(1.0, -1.0);
    paintscene_ = new QGraphicsScene(this);
    tfPreview_->setScene(paintscene_);
    tfPreview_->setFixedHeight(20.0);
    tfPreview_->setAlignment(Qt::AlignLeft);

    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->addWidget(zoomVSlider_, 0, 0);
    gridLayout->addWidget(tfEditorView_,    0, 1);
    gridLayout->addWidget(zoomHSlider_, 1, 1);
    gridLayout->addWidget(tfPreview_,   2, 1);
    gridLayout->addWidget(maskSlider_,  3, 1);
    gridLayout->addWidget(colorWheel_,  0, 2);
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
    QPixmap checkerBoard(10, 10);
    QPainter checkerBoardPainter(&checkerBoard);
    checkerBoardPainter.fillRect(0, 0, 5, 5, Qt::lightGray);
    checkerBoardPainter.fillRect(5, 0, 5, 5, Qt::darkGray);
    checkerBoardPainter.fillRect(0, 5, 5, 5, Qt::darkGray);
    checkerBoardPainter.fillRect(5, 5, 5, 5, Qt::lightGray);
    checkerBoardPainter.end();

    tfPixmap_ = new QPixmap(tfEditorView_->width(), 20);
    QPainter tfPainter(tfPixmap_);
    tfPainter.fillRect(0, 0, tfEditorView_->width(), 20, QBrush(checkerBoard));
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
    paintscene_->setForegroundBrush(QBrush(*tfPixmap_));
    this->update();
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
    paintscene_->setForegroundBrush(*gradient_);
    gradient_->setFinalStop(tfEditorView_->width(), 0.0);	
}

void TransferFunctionPropertyDialog::zoomHorizontally(int zoomHMin, int zoomHMax) {
    float zoomHMinF = static_cast<float>(zoomHMin)/static_cast<float>(arrayWidth_);
    float zoomHMaxF = static_cast<float>(zoomHMax)/static_cast<float>(arrayWidth_);
    tfProperty_->setZoomH(zoomHMinF, zoomHMaxF);
    tfEditorView_->setZoomH(zoomHMinF, zoomHMaxF);
    tfEditor_->setZoomRangeXMin(static_cast<float>(zoomHMin) / (static_cast<float>(arrayWidth_) - 1.0f));
    tfEditor_->setZoomRangeXMax(static_cast<float>(zoomHMax) / (static_cast<float>(arrayWidth_) - 1.0f));
    tfEditor_->repositionPoints();
}

void TransferFunctionPropertyDialog::zoomVertically(int zoomVMin, int zoomVMax) {
    float zoomVMinF = static_cast<float>(zoomVMin)/static_cast<float>(arrayHeight_);
    float zoomVMaxF = static_cast<float>(zoomVMax)/static_cast<float>(arrayHeight_);
    tfProperty_->setZoomV(zoomVMinF, zoomVMaxF);
    tfEditorView_->setZoomV(zoomVMinF, zoomVMaxF);
    tfEditor_->setZoomRangeYMin(1.f-zoomVMaxF);
    tfEditor_->setZoomRangeYMax(1.f-zoomVMinF);
    tfEditor_->repositionPoints();
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