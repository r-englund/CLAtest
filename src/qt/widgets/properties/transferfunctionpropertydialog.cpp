#include <inviwo/qt/widgets/properties/transferfunctionpropertydialog.h>

namespace inviwo {

	//TransferFunctionPropertyDialog::TransferFunctionPropertyDialog();

	TransferFunctionPropertyDialog::TransferFunctionPropertyDialog(TransferFunctionProperty* property, QWidget* parent) : 
InviwoDockWidget(tr("TransferFun"), parent),
property_(property)
{
	zoom_ = 0;
	width  = 255.0f;
	height = 100.0f;

	setObjectName("TransferFun");
	setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	generateWidget();
	updateFromProperty();
}

void TransferFunctionPropertyDialog::generateWidget(){

	QFrame* frame = new QFrame();
	setWidget(frame);

	QVBoxLayout* vLayout = new QVBoxLayout();
	QHBoxLayout* hLayout = new QHBoxLayout();

	editor_ = new TransferFunctionEditor(&property_->get());
	addObservation(editor_);
	editor_->addObserver(this);
	editor_->setSceneRect(0,0,width,height);
	editor_->setBackgroundBrush(Qt::transparent);
	QDockWidget::connect(editor_,SIGNAL(selectionChanged()),this,SLOT(updateColorWheel()));
	QDockWidget::connect(editor_,SIGNAL(doubleClick()),this,SLOT(showColorDialog()));

	colorChange_ = false;
	colorWheel_ = new ColorWheel();
	colorDialog_ = new QColorDialog();
	colorDialog_->setWindowFlags(Qt::WindowStaysOnTopHint);
	QDockWidget::connect(colorWheel_,SIGNAL(colorChange(QColor)),this, SLOT(setPropertyValue()));
	QDockWidget::connect(colorDialog_,SIGNAL(currentColorChanged(QColor)),this,SLOT(setPropertyValueColorDialog()));

	editorview_ = new QGraphicsView(this);
	//editorview_->setFixedSize(width + 2, height + 2);
	editorview_->resize(width + 2, height + 2);
	editorview_->scale(1.0, -1.0);
	editorview_->setScene(editor_);
	editorview_->viewport()->installEventFilter(this);
	editorview_->setDragMode(QGraphicsView::RubberBandDrag);
	editorview_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	editorview_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//editorview_->fitInView(editor_->sceneRect(), Qt::KeepAspectRatio);

	paintscene_ = new QGraphicsScene(this);
	paintview_ = new QGraphicsView(this);
	paintview_->setScene(paintscene_);
	paintview_->setFixedSize(width + 2, 52);
	paintview_->scale(1.0, -1.0);


	QHBoxLayout* hSliderLayout = new QHBoxLayout();
	spinBoxMin_ = new QSpinBox(this);
	spinBoxMin_->setFixedWidth(50);
	spinBoxMin_->setMaximum(255);
	hSliderLayout->addWidget(spinBoxMin_);

	zoomSlider_ = new RangeSliderQt(Qt::Horizontal, this);
	zoomSlider_->setRange(0, 256);
	zoomSlider_->setMinValue(0);
	zoomSlider_->setMaxValue(256);
	hSliderLayout->addWidget(zoomSlider_);

	spinBoxMax_ = new QSpinBox(this);
	spinBoxMax_->setFixedWidth(50);
	spinBoxMax_->setMaximum(255);
	hSliderLayout->addWidget(spinBoxMax_);

	connect(zoomSlider_, SIGNAL(valuesChanged(int,int)), this, SLOT(updateFromSlider(int,int)));
	connect(spinBoxMin_, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBoxMin(int)));
	connect(spinBoxMax_, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBoxMax(int)));

	vLayout->addWidget(editorview_);
	vLayout->addLayout(hSliderLayout);
	vLayout->addWidget(paintview_);
	hLayout->addLayout(vLayout);
	hLayout->addWidget(colorWheel_);

	frame->setLayout(hLayout);

	gradient_ = new QLinearGradient(-128.0f, 0.0f, 127.0f, 0.0f);
	stops_ = new QVector<QGradientStop>();

}

TransferFunctionPropertyDialog::~TransferFunctionPropertyDialog(){
	delete editor_;
	delete gradient_;
	stops_->clear();
	delete stops_;
	delete colorDialog_;
	delete colorWheel_;
}

void TransferFunctionPropertyDialog::setPropertyValue(){
	QColor color = colorWheel_->color();
	setPointColor(color);
}


void TransferFunctionPropertyDialog::setPropertyValueColorDialog(){
	QColor color = colorDialog_->currentColor();
	setPointColor(color);
	colorChange_ =true;
	updateColorWheel();
	colorChange_ = false;
}


void TransferFunctionPropertyDialog::updateFromProperty(){
	QGradientStop* temp = new QGradientStop();
	stops_->clear();
	const vec4* col;
	if (property_->get().getNumberOfDataPoints() > 0){
		for (int i = 0; i < (int)property_->get().getNumberOfDataPoints(); i++){
			col = property_->get().getPoint(i)->getRgba();
			temp->first = property_->get().getPoint(i)->getPos()->x / 255.0f;
			temp->second = QColor::fromRgbF(col->r, col->g, col->b, 1.0f);
			stops_->push_front(*temp);
		}
	}
	else{
		const vec4* dataArray = static_cast<const vec4*>(property_->get().getData()->getRepresentation<ImageRAM>()->getData());
		temp->first = 0.0f;
		temp->second = QColor::fromRgbF(dataArray[0].r, dataArray[0].g, dataArray[0].b, 1.0f);
		stops_->push_front(*temp);
		temp->first = 1.0f;
		temp->second = QColor::fromRgbF(dataArray[255].r, dataArray[255].g, dataArray[255].b, 1.0f);
		stops_->push_front(*temp);
	}
	gradient_->setStops(*stops_);
	paintscene_->setForegroundBrush(*gradient_);
	delete temp;
	this->update();
	property_->invalidate();
	property_->customSet();
}

bool TransferFunctionPropertyDialog::eventFilter(QObject *object, QEvent *e)
{
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

		spinBoxMin_->setValue(floor(editorview_->mapToScene(0,0).x()));
		spinBoxMax_->setValue(floor(editorview_->mapToScene(editorview_->width(),0).x()));

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

	if (selection.size()==1 && dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(0))) {
		const vec4* pointColor = dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(0))->getPoint()->getRgba();
		colorWheel_->setColor(QColor(pointColor->x*255, pointColor->y*255, pointColor->z*255, pointColor->w*255));
		if (!colorChange_)
			colorDialog_->setCurrentColor(QColor(pointColor->x*255, pointColor->y*255, pointColor->z*255, pointColor->w*255));
	}
}

void TransferFunctionPropertyDialog::showColorDialog(){
	QList<QGraphicsItem *> selection = editor_->selectedItems();
	if (selection.size()==1 && dynamic_cast<TransferFunctionEditorControlPoint*>(selection.at(0))){
		colorDialog_->show();
	}     
}

void TransferFunctionPropertyDialog::updateFromSlider(int valMin, int valMax){
	if (valMax == 256){
		valMax = 255;
	}

	valMin = (valMin == 255) ? 254 : valMin;
	valMax = (valMax == 0) ? 1 : valMax;

	if(valMin != spinBoxMin_->value()){
		spinBoxMin_->setValue(valMin);
	}
	if(valMax != spinBoxMax_->value()){
		spinBoxMax_->setValue(valMax);
	}


}

void TransferFunctionPropertyDialog::updateFromSpinBoxMin(int val){
	if (val >= zoomSlider_->maxValue()){
		zoomSlider_->setMaxValue(val + 1.0);
	}
	zoomSlider_->setMinValue(val);

	float min = zoomSlider_->minValue();
	float max = zoomSlider_->maxValue();

	editorview_->resetMatrix();
	editorview_->scale(255.0/(max - min), -1.0);
	editorview_->setSceneRect(min, 0.0, max - min, 100.0);
}

void TransferFunctionPropertyDialog::updateFromSpinBoxMax(int val){
	if (val <= zoomSlider_->minValue()){
		zoomSlider_->setMinValue(val - 1.0);
	}
	if (val == 256){
		val = 255;
	}
	zoomSlider_->setMaxValue(val);

	float min = zoomSlider_->minValue();
	float max = zoomSlider_->maxValue();

	editorview_->resetMatrix();
	editorview_->scale(255.0/(max - min), -1.0);

	editorview_->centerOn((max-min)/2.0, 50.0);
	editorview_->setSceneRect(min, 0.0, max - min, 100.0);
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
} // namespace