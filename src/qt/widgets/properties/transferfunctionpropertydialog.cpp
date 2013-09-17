#include <inviwo/qt/widgets/properties/transferfunctionpropertydialog.h>
#include <math.h>
#include <QComboBox>

namespace inviwo {

	TransferFunctionPropertyDialog::TransferFunctionPropertyDialog(TransferFunctionProperty* property, QWidget* parent) : 
InviwoDockWidget(tr("TransferFun"), parent),
property_(property)
{
	zoom_ = 0;

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

	editorview_ = new TransferFunctionEditorView();
	editorview_->setParent(this);

	//editorview_->scale(1.0, -1.0);
	editorview_->setMinimumSize(minWidth, minHeight);
	editorview_->viewport()->installEventFilter(this);

	editorview_->setDragMode(QGraphicsView::NoDrag);
	editorview_->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
	editorview_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	editorview_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	QDockWidget::connect(editorview_, SIGNAL(resized()), this, SLOT(editorViewResized()));

	vLayout->addWidget(editorview_);

	editor_ = new TransferFunctionEditor(&property_->get(), editorview_);
	addObservation(editor_);
	editor_->addObserver(this);

	editor_->setSceneRect(0,0,minWidth, minHeight);

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
	paintview_->scale(1.0, -1.0);

	QHBoxLayout* hSliderLayout = new QHBoxLayout();
	zoomSpinBoxMin_ = new QSpinBox(this);
	zoomSpinBoxMin_->setFixedWidth(70);
	zoomSpinBoxMin_->setMaximum(arrayWidth_);
	hSliderLayout->addWidget(zoomSpinBoxMin_);

	zoomSlider_ = new RangeSliderQt(Qt::Horizontal, this);
	zoomSlider_->setRange(0, arrayWidth_);
	zoomSlider_->setMinValue(0);
	zoomSlider_->setMaxValue(arrayWidth_);
	hSliderLayout->addWidget(zoomSlider_);

	zoomSpinBoxMax_ = new QSpinBox(this);
	zoomSpinBoxMax_->setFixedWidth(70);
	zoomSpinBoxMax_->setMaximum(arrayWidth_ - 1);
	hSliderLayout->addWidget(zoomSpinBoxMax_);

	connect(zoomSlider_, SIGNAL(valuesChanged(int,int)), this, SLOT(updateFromSlider(int,int)));
	connect(zoomSpinBoxMin_, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBoxMin(int)));
	connect(zoomSpinBoxMax_, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBoxMax(int)));


	QComboBox* bitBox = new QComboBox();
	bitBox->addItem("8", 8);
	bitBox->addItem("12", 12);
	bitBox->addItem("16", 16);

	connect(bitBox, SIGNAL(currentIndexChanged(int)), this, SLOT(arrayWidthChanged(int)));

	vLayout->addWidget(paintview_);
	vLayout->addLayout(hSliderLayout);

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
	property_->customSet();
}

bool TransferFunctionPropertyDialog::eventFilter(QObject *object, QEvent *e){
	std::stringstream ss;
	if (e->type() == QEvent::Wheel){
		const QPoint pos = static_cast<QWheelEvent*>(e)->pos();

		double scaleFactor = 1.10; ///< Zoom in/out by 10%
		if (static_cast<QWheelEvent*>(e)->delta() > 0){
			editorview_->scale(scaleFactor, 1.0);
		} 
		else if (editorview_->matrix().m11() > 1.0 ){
			editorview_->scale(1.0 / scaleFactor, 1.0);
		}

		QPointF focus = editorview_->mapToScene(pos);
		editorview_->centerOn(focus);

		zoomSpinBoxMin_->setValue(floor(editorview_->mapToScene(0,0).x()));
		zoomSpinBoxMax_->setValue(floor(editorview_->mapToScene(editorview_->width(),0).x()));

		e->accept();
		this->update();
		editorview_->update();
		return true;
	}
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

void TransferFunctionPropertyDialog::updateFromSlider(int valMin, int valMax){
	if (valMax == arrayWidth_){
		valMax = arrayWidth_ - 1;
	}

	valMin = (valMin == arrayWidth_) ? arrayWidth_ - 2 : valMin;
	valMax = (valMax == 0) ? 1 : valMax;

	if(valMin != zoomSpinBoxMin_->value()){
		zoomSpinBoxMin_->setValue(valMin);
	}
	if(valMax != zoomSpinBoxMax_->value()){
		zoomSpinBoxMax_->setValue(valMax);
	}
}

void TransferFunctionPropertyDialog::updateFromSpinBoxMin(int v){
	int val = zoomSpinBoxMin_->value();
	if (val >= zoomSlider_->maxValue()){
		zoomSlider_->setMaxValue(val + 1.0);
	}
	zoomSlider_->setMinValue(val);

	float zoomRangeMin = zoomSlider_->minValue();
	float zoomRangeMax = zoomSlider_->maxValue();
	float viewRangeMin = zoomRangeMin / arrayWidth_ * editorview_->width();
	float viewRangeMax = zoomRangeMax / arrayWidth_ * editorview_->width();
	float viewScale = arrayWidth_ / (zoomRangeMax - zoomRangeMin);

	LogInfo(viewScale);
	viewScale = (viewScale > 25.6) ? 25.6 : viewScale;

	editorview_->resetMatrix();
	editorview_->scale(viewScale, -1.0);
	editorview_->setSceneRect(viewRangeMin, 0.0, viewRangeMax, 100.0);
}

void TransferFunctionPropertyDialog::updateFromSpinBoxMax(int v){
	int val = zoomSpinBoxMax_->value();
	if (val <= zoomSlider_->minValue()){
		zoomSlider_->setMinValue(val - 1.0);
	}
	if (val == arrayWidth_){
		val = arrayWidth_ - 1;
	}
	zoomSlider_->setMaxValue(val);

	float zoomRangeMin = zoomSlider_->minValue();
	float zoomRangeMax = zoomSlider_->maxValue();
	float viewRangeMin = zoomRangeMin / arrayWidth_ * editorview_->width();
	float viewRangeMax = zoomRangeMax / arrayWidth_ * editorview_->width();
	float viewScale = arrayWidth_ / (zoomRangeMax - zoomRangeMin);

	editorview_->resetMatrix();
	editorview_->scale(viewScale, -1.0);
	editorview_->setSceneRect(viewRangeMin, 0.0, viewRangeMax, 100.0);
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

void TransferFunctionPropertyDialog::arrayWidthChanged(int index){
	int prevWidth = arrayWidth_;
	arrayWidth_ = pow(2.0, 8 + index * 4);

	float factor = (float)arrayWidth_ / (float)prevWidth;
	int minValue =  zoomSpinBoxMin_->value();
	int maxValue = zoomSpinBoxMax_->value();
	int newSliderMin = ceil(zoomSpinBoxMin_->value() * factor);
	int newSliderMax = ceil(zoomSpinBoxMax_->value() * factor);

	zoomSpinBoxMin_->blockSignals(true);
	zoomSpinBoxMin_->blockSignals(true);
	zoomSlider_->blockSignals(true);

	zoomSlider_->setRange(0, arrayWidth_);
	zoomSpinBoxMin_->setRange(0, arrayWidth_);
	zoomSpinBoxMax_->setRange(0, arrayWidth_);

	zoomSlider_->setValue(newSliderMin, newSliderMax);

	zoomSpinBoxMin_->setValue(newSliderMin);
	zoomSpinBoxMax_->setValue(newSliderMax);

	zoomSpinBoxMin_->blockSignals(false);
	zoomSpinBoxMin_->blockSignals(false);
	zoomSlider_->blockSignals(false);
}
} // namespace