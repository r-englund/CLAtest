#include <inviwo/qt/widgets/properties/transferpropertywidgetqt.h>

namespace inviwo {
	const std::string TransferPropertyWidgetQt::logSource_ = "TransferPropertyWidget";
	TransferPropertyWidgetQt::TransferPropertyWidgetQt(TransferProperty* property) : property_(property) {
		generateWidget();
		updateFromProperty();
	}

	void TransferPropertyWidgetQt::generateWidget() {
		QVBoxLayout *	vLayout = new QVBoxLayout();
		scene = new QGraphicsScene(QRect(0,0,220,220));
		view = new QGraphicsView(this);

		view->setScene(scene); 
		view->setFixedSize(240,240);
		scene->setBackgroundBrush(Qt::green);

		scene->addRect(0,0,20,20);
		scene->addRect(200,0,20,20);
		scene->addRect(0,200,20,20);
		scene->addRect(200,200,20,20);

		vLayout->addWidget(view);
		view->show();
		setLayout(vLayout);

		checkBox_ = new QCheckBox();
		connect(checkBox_, SIGNAL(pressed()), this, SLOT(setPropertyValue()));
				vLayout->addWidget(checkBox_);
	}

	void TransferPropertyWidgetQt::mousePressEvent(QGraphicsSceneMouseEvent * e)	{
		LogInfo("Widgetpress!");
	}

	void TransferPropertyWidgetQt::setPropertyValue() {
		property_->set(checkBox_->isChecked());
	}

	void TransferPropertyWidgetQt::updateFromProperty() {
		checkBox_->setChecked(property_->get());
		if (property_->get())
		{
			scene->setBackgroundBrush(Qt::green);
		}
		else
		{
			scene->setBackgroundBrush(Qt::red);
		}
	}

} // namespace
