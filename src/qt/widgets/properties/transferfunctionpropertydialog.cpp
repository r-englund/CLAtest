#include <inviwo/qt/widgets/properties/transferfunctionpropertydialog.h>

#include <math.h>
#include <QComboBox>

namespace inviwo {

    TransferFunctionPropertyDialog::TransferFunctionPropertyDialog(TransferFunctionProperty* property, QWidget* parent) : 
InviwoDockWidget(tr("TransferFun"), parent),
property_(property)
{
    zoomFactor_ = 1;

    setObjectName("TransferFun");
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
    QHBoxLayout* hLayout = new QHBoxLayout();

    zoomSpinBoxSlider_ = new SpinBoxRangeSliderQt();
    maskSpinBoxSlider_ = new SpinBoxRangeSliderQt();

    verticalZoomSlider_ = new RangeSliderQt(Qt::Vertical, this);
    verticalZoomSlider_->setRange(999, 0);
    verticalZoomSlider_->setMinValue(0);
    verticalZoomSlider_->setMaxValue(300);

    //zoomProp_ = new IntMinMaxProperty("zoom","zoom",0,arrayWidth_,0,arrayWidth_);
    zoomProp_ = new IntMinMaxProperty("zoom","zoom",0,100);
    zoomProp_->setRangeMax(20);
    zoomProp_->get().x = 7;


    zoomPropWidget_ = new IntMinMaxPropertyWidgetQt(zoomProp_);

    connect(zoomSpinBoxSlider_, SIGNAL(valuesChanged()), this, SLOT(zoomChanged()));
    connect(maskSpinBoxSlider_, SIGNAL(valuesChanged()), this, SLOT(maskChanged()));

    connect(verticalZoomSlider_, SIGNAL(valuesChanged(int, int)), this, SLOT(vertZoomChanged(int, int)));

    connect(zoomPropWidget_, SIGNAL(valuesChanged()), this, SLOT(zoomChanged()));

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
    this->zoomChanged();

    editor_->setBackgroundBrush(Qt::transparent);
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

    QComboBox* bitBox = new QComboBox();
    bitBox->addItem("8", 8);
    bitBox->addItem("12", 12);
    bitBox->addItem("16", 16);

    connect(bitBox, SIGNAL(currentIndexChanged(int)), this, SLOT(bitRangeChanged(int)));

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->addWidget(verticalZoomSlider_);
    hLay->addWidget(editorview_);

    vLayout->addLayout(hLay);
    vLayout->addWidget(paintview_);	

    vLayout->addWidget(zoomSpinBoxSlider_);
    vLayout->addWidget(maskSpinBoxSlider_);

    vLayout->addWidget(zoomPropWidget_);
    //vLayout->addWidget(maskPropWidget_);

    vLayout->addWidget(bitBox);

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
    property_->propertyModified();
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

void TransferFunctionPropertyDialog::bitRangeChanged(int index){
    int prevWidth = arrayWidth_;
    arrayWidth_ = pow(2.0, 8 + index * 4);

    float scaleFactor_ = (float)arrayWidth_ / (float)prevWidth;
    int minValue = zoomSpinBoxSlider_->getMinValue();
    int maxValue = zoomSpinBoxSlider_->getMaxValue();
    int newSliderMin = ceil(minValue * scaleFactor_);
    int newSliderMax = ceil(maxValue * scaleFactor_);

    zoomSpinBoxSlider_->setMaxRange(arrayWidth_);
    zoomSpinBoxSlider_->setMinValue(newSliderMin);
    zoomSpinBoxSlider_->setMaxValue(newSliderMax);

    minValue = maskSpinBoxSlider_->getMinValue();
    maxValue = maskSpinBoxSlider_->getMaxValue();

    minValue = maskProp_->getRangeMin();
    maxValue = maskProp_->getRangeMax();

    newSliderMin = ceil(minValue * scaleFactor_);
    newSliderMax = ceil(maxValue * scaleFactor_);

    maskSpinBoxSlider_->setMaxRange(arrayWidth_);
    maskSpinBoxSlider_->setMinValue(newSliderMin);
    maskSpinBoxSlider_->setMaxValue(newSliderMax);
}

void TransferFunctionPropertyDialog::zoomChanged(){
    float zoomMinX = (float)zoomSpinBoxSlider_->getMinValue() / ((float)arrayWidth_ - 1.0);
    float zoomMaxX = (float)zoomSpinBoxSlider_->getMaxValue() / ((float)arrayWidth_ - 1.0);

    editor_->setZoomRangeXMin(zoomMinX);
    editor_->setZoomRangeXMax(zoomMaxX);

    editor_->repositionPoints();
    maskChanged();
}

void TransferFunctionPropertyDialog::vertZoomChanged(int max, int min){
    min = (min < 0) ? 0 : min;
    max = (max < 1) ? 1 : max;
    min = (min > 998) ? 998 : min;
    max = (max > 999) ? 999 : max;
    editor_->setZoomRangeYMin((float)min / 999.0);
    editor_->setZoomRangeYMax((float)max / 999.0);
    editor_->repositionPoints();
}

void TransferFunctionPropertyDialog::maskChanged(){
    int width = editorview_->width();
    int maskMin = maskSpinBoxSlider_->getMinValue();
    int maskMax = maskSpinBoxSlider_->getMaxValue();
    int zoomMin = zoomSpinBoxSlider_->getMinValue();
    int zoomMax = zoomSpinBoxSlider_->getMaxValue();

    float maskMinScenePos = width * ((float)maskMin - (float)zoomMin)/((float)zoomMax - (float)zoomMin);
    float maskMaxScenePos = width * ((float)maskMax - (float)zoomMin)/((float)zoomMax - (float)zoomMin);
    editorview_->setMaskMin(maskMinScenePos);
    editorview_->setMaskMax(maskMaxScenePos);

    float propertyMaskMinPos = (float)maskSpinBoxSlider_->getMinValue() / (float)arrayWidth_;
    float propertyMaskMaxPos = (float)maskSpinBoxSlider_->getMaxValue() / (float)arrayWidth_;

    propertyMaskMinPos = (propertyMaskMinPos < 0.0) ? 0.0 : propertyMaskMinPos;
    propertyMaskMinPos = (propertyMaskMinPos > 1.0) ? 1.0 : propertyMaskMinPos;

    propertyMaskMaxPos = (propertyMaskMaxPos < 0.0) ? 0.0 : propertyMaskMaxPos;
    propertyMaskMaxPos = (propertyMaskMaxPos > 1.0) ? 1.0 : propertyMaskMaxPos;

    property_->get().setMaskMin(propertyMaskMinPos);
    property_->get().setMaskMax(propertyMaskMaxPos);

    editorview_->update();
    property_->get().calcTransferValues();
    property_->propertyModified();
}

const int TransferFunctionPropertyDialog::getArrayWidth() { 
    return arrayWidth_; 
}

} // namespace