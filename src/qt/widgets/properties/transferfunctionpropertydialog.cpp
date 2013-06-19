#include <inviwo/qt/widgets/properties/transferfunctionpropertydialog.h>

namespace inviwo {

	TransferFunctionPropertyDialog::TransferFunctionPropertyDialog(TransferFunctionProperty* property) : property_(property){
		zoom_ = 0;
		width  = 255.0f;
		height = 100.0f;
		generateWidget();
		updateFromProperty();
	}

	TransferFunctionPropertyDialog::~TransferFunctionPropertyDialog(){
		delete editor_;
		delete gradient_;
		stops_->clear();
		delete stops_;
		delete colorDialog_;
	}

	void TransferFunctionPropertyDialog::generateWidget(){
		colorDialog_ = new QColorDialog();
		colorDialog_->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
		connect(colorDialog_,SIGNAL(currentColorChanged(QColor)),this, SLOT(setPropertyValue()));

		QVBoxLayout* vLayout = new QVBoxLayout();
		setLayout(vLayout);

		editor_ = new TransferFunctionEditor(this, &property_->get());
		editor_->setSceneRect(0,0,width,height);
		editor_->setBackgroundBrush(Qt::transparent);

		editorview_ = new QGraphicsView();
		editorview_->setFixedSize(width + 2, height + 2);
		editorview_->scale(1, -1);
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
		paintview_->scale(1, -1);

		vLayout->addWidget(editorview_);
		vLayout->addWidget(paintview_);
		vLayout->addWidget(colorDialog_);

		gradient_ = new QLinearGradient(-128.0f, 0.0f, 127.0f, 0.0f);
		stops_ = new QVector<QGradientStop>();
	}


	void TransferFunctionPropertyDialog::setPropertyValue(){
		vec3* newRgb;
		QColor color = colorDialog_->currentColor();

		for (int i = 0; i < (int)property_->get().getNumberOfDataPoints() ; i++){
			if (property_->get().getPoint(i)->isSelected()){
				LogInfo(property_->get().getPoint(i)->getPos()->x);
				newRgb = new vec3(color.redF(),color.greenF(),color.blueF());
				property_->get().getPoint(i)->setRgb(newRgb);
			}
		}
		editor_->update();
		updateFromProperty();
		(&property_->get())->calcTransferValues();
	}

	void TransferFunctionPropertyDialog::updateFromProperty(){
		QGradientStop* temp = new QGradientStop();
		stops_->clear();
		const vec4* col;
		vec4 hej;
		if (property_->get().getNumberOfDataPoints() > 0){
			for (int i = 0; i < (int)property_->get().getNumberOfDataPoints(); i++){
				col = property_->get().getPoint(i)->getRgba();
				temp->first = property_->get().getPoint(i)->getPos()->x / 255.0f;
				temp->second = QColor::fromRgbF(col->r, col->g, col->b, 1.0f);
				stops_->push_front(*temp);
			}
		}
		else{
			vec4* dataArray = static_cast<vec4*>(property_->get().getData()->getEditableRepresentation<ImageRAMVec4float32>()->getData());
			hej = dataArray[0];
			temp->first = 0.0f;
			temp->second = QColor::fromRgbF(hej.r, hej.g, hej.b, 1.0f);
			LogInfo(hej.r);
			stops_->push_front(*temp);
			hej = dataArray[255];
			temp->first = 1.0f;
			temp->second = QColor::fromRgbF(hej.r, hej.g, hej.b, 1.0f);
			stops_->push_front(*temp);
			LogInfo(hej.r);
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

			double scaleFactor = 1.20; ///< Zoom in/out by 10%
			if (static_cast<QWheelEvent*>(e)->delta() > 0){
				zoom_++;
				editorview_->scale(scaleFactor, scaleFactor);
			} 
			else if (zoom_ > 0){
				zoom_--;
				editorview_->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
			}

			QPointF focus = editorview_->mapToScene(pos);
			editorview_->centerOn(focus);

			e->accept();
			this->update();
			editorview_->update();
			return true;
		}
		return false;
	}
} // namespace