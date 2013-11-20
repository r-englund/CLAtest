#include <inviwo/qt/widgets/properties/buttonpropertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>


namespace inviwo {

ButtonPropertyWidgetQt::ButtonPropertyWidgetQt(ButtonProperty *property) : property_(property){
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
	generateWidget();
    updateFromProperty();

	}

void ButtonPropertyWidgetQt::generateWidget() {
	QHBoxLayout* hLayout = new QHBoxLayout();
	button_ = new QPushButton();
    button_->setText(QString::fromStdString(property_->getDisplayName()));
	connect(button_, SIGNAL(released()), this, SLOT(handleButton()));
	hLayout->addWidget(button_);
	setLayout(hLayout);
}

void ButtonPropertyWidgetQt::handleButton(){
    property_->invokeMemberFunctions();
}
void ButtonPropertyWidgetQt::updateFromProperty() {}

QPushButton* ButtonPropertyWidgetQt::getButton() {
    return button_;
}


} //namespace