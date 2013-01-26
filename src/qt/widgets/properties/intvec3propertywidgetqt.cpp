#include "inviwo/qt/widgets/properties/intvec3propertywidgetqt.h"
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

IntVec3PropertyWidgetQt::IntVec3PropertyWidgetQt(IntVec3Property *property) : property_(property) {
	generateWidget();
	updateFromProperty();
	}

void IntVec3PropertyWidgetQt::generateWidget() {
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));

	QWidget* sliderWidget = new QWidget();
	QVBoxLayout* vLayout = new QVBoxLayout();
	sliderWidget->setLayout(vLayout);
	sliderX_ = new QSlider(Qt::Horizontal);
	vLayout->addWidget(sliderX_);
	sliderY_ =new QSlider(Qt::Horizontal);
	vLayout->addWidget(sliderY_);
	sliderZ_ =new QSlider(Qt::Horizontal);
	vLayout->addWidget(sliderZ_);
	hLayout->addWidget(sliderWidget);
	connect(sliderX_, SIGNAL(valueChanged(int)), this, SLOT(setXValue(int)));
	connect(sliderY_, SIGNAL(valueChanged(int)), this, SLOT(setYValue(int)));
	connect(sliderZ_, SIGNAL(valueChanged(int)), this, SLOT(setZValue(int)));
	setLayout(hLayout);
}

void IntVec3PropertyWidgetQt::setXValue(int value) {
	ivec3 valueVec3 = property_->get();
	valueVec3.x = value;
	property_->set(valueVec3);
}

void IntVec3PropertyWidgetQt::setYValue(int value) {
	ivec3 valueVec3 = property_->get();
	valueVec3.y = value;
	property_->set(valueVec3);
}

void IntVec3PropertyWidgetQt::setZValue(int value) {
	ivec3 valueVec3 = property_->get();
	valueVec3.z = value;
	property_->set(valueVec3);
}

void IntVec3PropertyWidgetQt::setPropertyValue() {
	setXValue(sliderX_->value());
	setYValue(sliderY_->value());
	setZValue(sliderZ_->value());
}

void IntVec3PropertyWidgetQt::updateFromProperty() {
	ivec3 valueVec3 = property_->get();
	sliderX_->setValue(sliderX_->minimum()+valueVec3.x);
	sliderY_->setValue(sliderY_->minimum()+valueVec3.y);
	sliderZ_->setValue(sliderZ_->minimum()+valueVec3.z);
};


} //namespace