#include <inviwo/qt/widgets/properties/buttonpropertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

ButtonPropertyWidgetQt::ButtonPropertyWidgetQt(ButtonProperty *property) : property_(property){
	generateWidget();
	updateFromProperty();
	}

void ButtonPropertyWidgetQt::generateWidget() {
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
	button_ = new QPushButton();
	connect(button_, SIGNAL(released()), this, SLOT(handleButton()));
	hLayout->addWidget(button_);
	setLayout(hLayout);
}

void ButtonPropertyWidgetQt::handleButton()
{
    // change the text
    button_->setText("Pressed the button");
}

void ButtonPropertyWidgetQt::updateFromProperty() {
	std::string valueStr = property_->get();
	QString str = QString::fromUtf8(valueStr.c_str());
	button_->setText(str);
}




} //namespace