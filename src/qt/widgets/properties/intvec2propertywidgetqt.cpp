#include "inviwo/qt/widgets/properties/intvec2propertywidgetqt.h"
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

IntVec2PropertyWidgetQt::IntVec2PropertyWidgetQt(IntVec2Property *property) : property_(property) {
	generateWidget();
	updateFromProperty();
	}

void IntVec2PropertyWidgetQt::generateWidget() {
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));

	QWidget* sliderWidget = new QWidget();
	QVBoxLayout* vLayout = new QVBoxLayout();
	sliderWidget->setLayout(vLayout);
	sliderX_ = new QSlider(Qt::Horizontal);
	vLayout->addWidget(sliderX_);
	sliderY_ =new QSlider(Qt::Horizontal);
	vLayout->addWidget(sliderY_);
	hLayout->addWidget(sliderWidget);
	connect(sliderX_, SIGNAL(valueChanged(int)), this, SLOT(setXValue(int)));
	connect(sliderY_, SIGNAL(valueChanged(int)), this, SLOT(setYValue(int)));
	setLayout(hLayout);
}

void IntVec2PropertyWidgetQt::setXValue(int value) {
	ivec2 valueVec2 = property_->get();
	valueVec2.x = value;
	property_->set(valueVec2);
}

void IntVec2PropertyWidgetQt::setYValue(int value) {
	ivec2 valueVec2 = property_->get();
	valueVec2.y = value;
	property_->set(valueVec2);
}

void IntVec2PropertyWidgetQt::setPropertyValue() {
	setXValue(sliderX_->value());
	setYValue(sliderY_->value());
}

void IntVec2PropertyWidgetQt::updateFromProperty() {
	ivec2 valueVec2 = property_->get();
	sliderX_->setValue(sliderX_->minimum()+valueVec2.x);
	sliderY_->setValue(sliderY_->minimum()+valueVec2.y);
};


} //namespace