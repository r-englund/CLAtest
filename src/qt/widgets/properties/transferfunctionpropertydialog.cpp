#include <inviwo/qt/widgets/properties/transferfunctionpropertydialog.h>

namespace inviwo {

	TransferFunctionPropertyDialog::TransferFunctionPropertyDialog(TransferFunctionProperty* property) : property_(property){
		zoom_ = 0;
		transferFunction_ = &property_->get();
		data_ = static_cast<vec4*>(transferFunction_->getData()->getEditableRepresentation<ImageRAMVec4float32>()->getData());
		generateWidget();
		updateFromProperty();
	}

	void TransferFunctionPropertyDialog::generateWidget(){
		colorDialog_ = new QColorDialog();
		colorDialog_->setOptions(QColorDialog::DontUseNativeDialog | QColorDialog::NoButtons);
		connect(colorDialog_,SIGNAL(currentColorChanged(QColor)),this, SLOT(setPropertyValue()));

		QVBoxLayout* vLayout = new QVBoxLayout();
		setLayout(vLayout);

		editor_ = new TransferFunctionEditor(this, transferFunction_, &points_);
		editor_->setSceneRect(0,0,255,100);
		editor_->setBackgroundBrush(Qt::transparent);

		editorview_ = new QGraphicsView(this);
		editorview_ = new QGraphicsView();
		editorview_->setFixedSize(257, 102);
		editorview_->scale(1, -1);
		editorview_->setScene(editor_);

		paintscene_ = new QGraphicsScene(this);
		paintview_ = new QGraphicsView(this);
		paintview_->setScene(paintscene_);
		paintview_->setFixedSize(257, 52);
		paintview_->scale(1, -1);

		vLayout->addWidget(editorview_);
		vLayout->addWidget(paintview_);
		vLayout->addWidget(colorDialog_);

		gradient_ = new QLinearGradient(-128.0f, 0.0f, 127.0f, 0.0f);
		stops_ = new QVector<QGradientStop>();
	}

	void TransferFunctionPropertyDialog::wheelEvent(QWheelEvent* e) {
		double scaleFactor = 1.10; ///< Zoom in/out by 10%
		if (e->delta() > 0){
			zoom_++;
			editorview_->scale(scaleFactor, scaleFactor);
		} else if (zoom_ > 0){
			zoom_--;
			editorview_->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
		}
		if(zoom_ == 0){
			editorview_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			editorview_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		}
		else{
			editorview_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
			editorview_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		}

		editorview_->setSceneRect(0,0,255,100);
		e->accept();
		this->update();
		editorview_->update();
	}

	void TransferFunctionPropertyDialog::setPropertyValue(){
		vec3* newRgb;
		QColor color = colorDialog_->currentColor();
		for (int i = 0; i < (int)points_.size() ; i++){
			if (points_[i]->isSelected()){
				newRgb = new vec3(color.redF(),color.greenF(),color.blueF());
				points_[i]->getPoint()->setRgb(newRgb);
			}
		}
		editor_->update();
		updateFromProperty();
		transferFunction_->calcTransferValues();
		property_->invalidate();
		emit modified();
	}

	void TransferFunctionPropertyDialog::updateFromProperty(){
		stops_->clear();
		if (points_.size() > 0){
			for (int i = 0; i < (int)points_.size(); i++){
				const vec4* col = points_[i]->getPoint()->getRgba();
				temp_ = new QGradientStop();
				temp_->first = points_[i]->getPoint()->getPos()->x / 255.01f;
				temp_->second = QColor::fromRgbF(col->r, col->g, col->b, 1.0f);
				stops_->push_front(*temp_);
			}
			gradient_->setStops(*stops_);
			paintscene_->setForegroundBrush(*gradient_);
		}
		this->update();
		property_->invalidate();
	}
} // namespace