#include <inviwo/qt/widgets/properties/transferpropertywidgetqt.h>

namespace inviwo {
	const std::string TransferPropertyWidgetQt::logSource_ = "TransferPropertyWidget";
	TransferPropertyWidgetQt::TransferPropertyWidgetQt(TransferProperty* property) : property_(property) {
		generateWidget();
		updateFromProperty();
	}

	void TransferPropertyWidgetQt::generateWidget() {
        QVBoxLayout *	vLayout = new QVBoxLayout();

        editor = new TransferEditor();
        editor->setSceneRect(0,0,255,100);
        
        view = new QGraphicsView(this);
        view->setFixedSize(257, 102);        
        view->setScene(editor); 
        editor->setBackgroundBrush(Qt::transparent);

        view->setInteractive(true);

		vLayout->addWidget(view);
		view->show();
		setLayout(vLayout);

		checkBox_ = new QCheckBox();
		connect(checkBox_, SIGNAL(pressed()), this, SLOT(setPropertyValue()));
				vLayout->addWidget(checkBox_);
	}

	void TransferPropertyWidgetQt::setPropertyValue() {
		property_->set(checkBox_->isChecked());
	}

	void TransferPropertyWidgetQt::updateFromProperty() {
		checkBox_->setChecked(property_->get());
		if (property_->get())
		{
			//editor->setBackgroundBrush(Qt::green);
		}
		else
		{
			//editor->setBackgroundBrush(Qt::red);
		}
	}

} // namespace
