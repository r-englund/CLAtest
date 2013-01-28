
#include <inviwo/qt/widgets/properties/intvec4propertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

IntVec4PropertyWidgetQt::IntVec4PropertyWidgetQt(IntVec4Property *property) : property_(property) {
	generateWidget();
	updateFromProperty();
	}

void IntVec4PropertyWidgetQt::generateWidget() {
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
	sliderW_ =new QSlider(Qt::Horizontal);
	vLayout->addWidget(sliderW_);
	hLayout->addWidget(sliderWidget);
	connect(sliderX_, SIGNAL(valueChanged(int)), this, SLOT(setXValue(int)));
	connect(sliderY_, SIGNAL(valueChanged(int)), this, SLOT(setYValue(int)));
	connect(sliderZ_, SIGNAL(valueChanged(int)), this, SLOT(setZValue(int)));
	connect(sliderW_, SIGNAL(valueChanged(int)), this, SLOT(setWValue(int)));
	setLayout(hLayout);
}

void IntVec4PropertyWidgetQt::setXValue(int value) {
	ivec4 valueVec4 = property_->get();
	valueVec4.x = value;
	property_->set(valueVec4);
}

void IntVec4PropertyWidgetQt::setYValue(int value) {
	ivec4 valueVec4 = property_->get();
	valueVec4.y = value;
	property_->set(valueVec4);
}

void IntVec4PropertyWidgetQt::setZValue(int value) {
	ivec4 valueVec4 = property_->get();
	valueVec4.z = value;
	property_->set(valueVec4);
}
void IntVec4PropertyWidgetQt::setWValue(int value) {
	ivec4 valueVec4 = property_->get();
	valueVec4.w = value;
	property_->set(valueVec4);
}

void IntVec4PropertyWidgetQt::setPropertyValue() {
	setXValue(sliderX_->value());
	setYValue(sliderY_->value());
	setZValue(sliderZ_->value());
	setZValue(sliderW_->value());
}

void IntVec4PropertyWidgetQt::updateFromProperty() {
	ivec4 valueVec4 = property_->get();
	sliderX_->setValue(sliderX_->minimum()+valueVec4.x);
	sliderY_->setValue(sliderY_->minimum()+valueVec4.y);
	sliderZ_->setValue(sliderZ_->minimum()+valueVec4.z);
	sliderW_->setValue(sliderW_->minimum()+valueVec4.w);
};


} //namespace