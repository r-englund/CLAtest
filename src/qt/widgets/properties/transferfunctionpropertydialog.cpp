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

#include <math.h>

namespace inviwo {

TransferFunctionPropertyDialog::TransferFunctionPropertyDialog(TransferFunctionProperty* tfProperty, QWidget* parent) : 
    InviwoDockWidget(tr("Transfer Function"), parent),
    property_(tfProperty)
{
    setObjectName("Transfer Function");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    generateWidget();
    updateFromProperty();
}

void TransferFunctionPropertyDialog::generateWidget(){
    float minWidth  = 255.0f;
    float minHeight = 100.0f;

    arrayWidth_ = 256;

    QFrame* frame = new QFrame();
    setWidget(frame);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    vLayout->setContentsMargins(0,0,0,0);

    QHBoxLayout* hLayout = new QHBoxLayout();

    verticalZoomSlider_ = new RangeSliderQt(Qt::Vertical, this);
    int maxNumberOfValues = (property_->zoomVerticalProperty()->getRangeMax()-property_->zoomVerticalProperty()->getRangeMin())/property_->zoomVerticalProperty()->getIncrement();
    verticalZoomSlider_->setRange(0, static_cast<int>(maxNumberOfValues));
    int sliderMin = static_cast<int>((property_->zoomVerticalProperty()->get().x-property_->zoomVerticalProperty()->getRangeMin())*maxNumberOfValues);
    int sliderMax = static_cast<int>((property_->zoomVerticalProperty()->get().y-property_->zoomVerticalProperty()->getRangeMin())*maxNumberOfValues);
    verticalZoomSlider_->setValue(sliderMin, sliderMax);
    connect(verticalZoomSlider_, SIGNAL(valuesChanged(int, int)), this, SLOT(zoomVerticalChanged(int, int)));

    zoomPropWidget_ = new IntMinMaxPropertyWidgetQt(property_->zoomHorizontalProperty());
    connect(zoomPropWidget_, SIGNAL(modified()), this, SLOT(zoomHorizontalChanged()));

    maskPropWidget_ = new IntMinMaxPropertyWidgetQt(property_->maskProperty());
    connect(maskPropWidget_, SIGNAL(modified()), this, SLOT(maskChanged()));

    bitRangewidget_ = new OptionPropertyWidgetQt(property_->bitRangeProperty());
    connect(bitRangewidget_, SIGNAL(modified()), this, SLOT(bitRangeChanged()));

    editorview_ = new TransferFunctionEditorView();
    editorview_->setParent(this);

    editorview_->scale(1.0, -1.0);
    editorview_->setMinimumSize(minWidth, minHeight);
    editorview_->viewport()->installEventFilter(this);

    editorview_->setDragMode(QGraphicsView::NoDrag);
    editorview_->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    editorview_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editorview_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QDockWidget::connect(editorview_, SIGNAL(resized()), this, SLOT(editorViewResized()));

    editor_ = new TransferFunctionEditor(&property_->get(), editorview_);
    addObservation(editor_);
    editor_->addObserver(this);

    editor_->setSceneRect(0,0,minWidth, minHeight);
    this->zoomHorizontalChanged();

    QDockWidget::connect(editor_,SIGNAL(doubleClick()),this,SLOT(showColorDialog()));
    QDockWidget::connect(editor_,SIGNAL(selectionChanged()),this,SLOT(updateColorWheel()));
    editorview_->setScene(editor_);

    colorChange_ = false;
    colorWheel_ = new ColorWheel();
    colorDialog_ = new QColorDialog();
    colorDialog_->setWindowFlags(Qt::WindowStaysOnTopHint);
    QDockWidget::connect(colorWheel_,SIGNAL(colorChange(QColor)),this, SLOT(setPropertyValue()));
    QDockWidget::connect(colorDialog_,SIGNAL(currentColorChanged(QColor)),this,SLOT(setPropertyValueColorDialog()));

    paintscene_ = new QGraphicsScene(this);
    paintview_ = new QGraphicsView(this);
    paintview_->setScene(paintscene_);
    paintview_->setFixedHeight(10.0);
    paintview_->setAlignment(Qt::AlignLeft);

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->addWidget(verticalZoomSlider_);
    hLay->addWidget(editorview_);

    vLayout->addLayout(hLay);
    vLayout->addWidget(paintview_);

    vLayout->addWidget(zoomPropWidget_);
    vLayout->addWidget(maskPropWidget_);

    vLayout->addWidget(bitRangewidget_);

    hLayout->addLayout(vLayout);
    hLayout->addWidget(colorWheel_);

    frame->setLayout(hLayout);

    gradient_ = new QLinearGradient(0.0f, 0.0f, editorview_->width(), 0.0f);
    stops_ = new QVector<QGradientStop>();
}

TransferFunctionPropertyDialog::~TransferFunctionPropertyDialog(){
    delete editor_;
    delete colorWheel_;
    delete gradient_;
    stops_->clear();
    delete stops_;
    delete colorDialog_;
}

void TransferFunctionPropertyDialog::setPropertyValue(){
    QColor color = colorWheel_->color();
    setPointColor(color);
}

void TransferFunctionPropertyDialog::setPropertyValueColorDialog(){
    QColor color = colorDialog_->currentColor();
    setPointColor(color);
    colorChange_ = true;
    updateColorWheel();
    colorChange_ = false;
}

void TransferFunctionPropertyDialog::updateFromProperty(){
    QGradientStop* temp = new QGradientStop();
    stops_->clear();
    const vec4* color;
    if (property_->get().getNumberOfDataPoints() > 0){
        for (int i = 0; i < (int)property_->get().getNumberOfDataPoints(); i++){
            color = property_->get().getPoint(i)->getRgba();
            temp->first = property_->get().getPoint(i)->getPos()->x;
            temp->second = QColor::fromRgbF(color->r, color->g, color->b, 1.0f);
            stops_->push_front(*temp);
        }
    }
    else{
        const vec4* dataArray = static_cast<const vec4*>(property_->get().getData()->getRepresentation<ImageRAM>()->getData());
        temp->first = 0.0f;
        temp->second = QColor::fromRgbF(dataArray[0].r, dataArray[0].g, dataArray[0].b, 1.0f);
        stops_->push_front(*temp);
        temp->first = 1.0f;
        temp->second = QColor::fromRgbF(dataArray[arrayWidth_].r, dataArray[arrayWidth_].g, dataArray[arrayWidth_].b, 1.0f);
        stops_->push_front(*temp);
    }
    gradient_->setStops(*stops_);
    paintscene_->setForegroundBrush(*gradient_);
    gradient_->setFinalStop(editorview_->width(), 0.0);
    delete temp;
    this->update();
}

bool TransferFunctionPropertyDialog::eventFilter(QObject *object, QEvent *e){
    return false;
}

void TransferFunctionPropertyDialog::notify(){
    updateFromProperty();
}

void TransferFunctionPropertyDialog::updateColorWheel(){
    QList<QGraphicsItem *> selection = editor_->selectedItems();

    if (selection.size()== 1 && dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(0))) {
        const vec4* pointColor = dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(0))->getPoint()->getRgba();
        colorWheel_->setColor(QColor(pointColor->r * 255.0, pointColor->g * 255.0, pointColor->b * 255.0, pointColor->a * 255.0));
        if (!colorChange_){
            colorDialog_->setCurrentColor(QColor(pointColor->r * 255.0, pointColor->g * 255.0, pointColor->b * 255.0, pointColor->a * 255.0));
        }
    }
}

void TransferFunctionPropertyDialog::showColorDialog(){
    QList<QGraphicsItem *> selection = editor_->selectedItems();
    if (selection.size()==1 && dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(0))){
        colorDialog_->show();
    }     
}


void TransferFunctionPropertyDialog::setPointColor( QColor color ){
    vec3* newRgb;
    QList<QGraphicsItem *> selection = editor_->selectedItems();

    for (int i=0; i< selection.size(); i++) {
        if (dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(i))) {
            newRgb = new vec3(color.redF(),color.greenF(),color.blueF());
            dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(i))->getPoint()->setRgb(newRgb);
        }
    }

    editor_->update();
    updateFromProperty();
    (&property_->get())->calcTransferValues();
}


QVector<QGradientStop>* TransferFunctionPropertyDialog::getGradientStops(){
    return stops_;
}

void TransferFunctionPropertyDialog::editorViewResized(){
    gradient_->setStops(*stops_);
    paintscene_->setForegroundBrush(*gradient_);
    gradient_->setFinalStop(editorview_->width(), 0.0);	
}

void TransferFunctionPropertyDialog::bitRangeChanged(){
    int prevWidth = arrayWidth_;
    int bits = property_->bitRangeProperty()->get();
    arrayWidth_ = static_cast<int>(pow(2.f, bits));

    float scaleFactor_ = (float)arrayWidth_ / (float)prevWidth;

    ivec2 zoomValue = property_->zoomHorizontalProperty()->get();
    property_->zoomHorizontalProperty()->setRangeMax(arrayWidth_);
    property_->zoomHorizontalProperty()->set(ivec2(static_cast<int>(ceil(static_cast<float>(zoomValue.x) * scaleFactor_)), 
        static_cast<int>(ceil(static_cast<float>(zoomValue.y) * scaleFactor_))));
    zoomPropWidget_->updateFromProperty();

    ivec2 maskValue = property_->maskProperty()->get();
    property_->maskProperty()->setRangeMax(arrayWidth_);
    property_->maskProperty()->set(ivec2(static_cast<int>(ceil(static_cast<float>(maskValue.x) * scaleFactor_)), 
        static_cast<int>(ceil(static_cast<float>(maskValue.y) * scaleFactor_))));
    maskPropWidget_->updateFromProperty();
}

void TransferFunctionPropertyDialog::zoomHorizontalChanged(){
    vec2 zoom;
    ivec2 izoom = property_->zoomHorizontalProperty()->get();
    zoom.x = static_cast<float>(izoom.x) / (static_cast<float>(arrayWidth_) - 1.f);
    zoom.y = static_cast<float>(izoom.y) / (static_cast<float>(arrayWidth_) - 1.f);
    editor_->setZoomRangeXMin(zoom.x);
    editor_->setZoomRangeXMax(zoom.y);
    editor_->repositionPoints();
    maskChanged();
}

void TransferFunctionPropertyDialog::zoomVerticalChanged(int valMin, int valMax){
    int maxNumberOfValues = (property_->zoomVerticalProperty()->getRangeMax()-property_->zoomVerticalProperty()->getRangeMin())/property_->zoomVerticalProperty()->getIncrement();
    float valMinF = static_cast<float>(valMin)/static_cast<float>(maxNumberOfValues);
    float valMaxF = static_cast<float>(valMax)/static_cast<float>(maxNumberOfValues);
    property_->zoomVerticalProperty()->set(vec2(valMinF, valMaxF));
    editor_->setZoomRangeYMin(1.f-valMaxF);
    editor_->setZoomRangeYMax(1.f-valMinF);
    editor_->repositionPoints();
}

void TransferFunctionPropertyDialog::maskChanged(){
    int width = editorview_->width();
    ivec2 mask = property_->maskProperty()->get();
    ivec2 zoom = property_->zoomHorizontalProperty()->get();

    float maskMinScenePos = width * ((float)mask.x - (float)zoom.x)/((float)zoom.y - (float)zoom.x);
    float maskMaxScenePos = width * ((float)mask.y - (float)zoom.x)/((float)zoom.y - (float)zoom.x);
    editorview_->setMaskMin(maskMinScenePos);
    editorview_->setMaskMax(maskMaxScenePos);

    property_->updateMask(mask, arrayWidth_);

    editorview_->update();
    property_->get().calcTransferValues();
    property_->propertyModified();
}

const int TransferFunctionPropertyDialog::getArrayWidth() { 
    return arrayWidth_; 
}

} // namespace