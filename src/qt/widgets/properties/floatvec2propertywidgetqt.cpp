#include <inviwo/qt/widgets/properties/floatvec2propertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

FloatVec2PropertyWidgetQt::FloatVec2PropertyWidgetQt(FloatVec2Property *property) : property_(property) {
	generateWidget();
	updateFromProperty();
	}

void FloatVec2PropertyWidgetQt::generateWidget() {
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

void FloatVec2PropertyWidgetQt::setXValue(int value) {
    float valuef = (static_cast<float>(value)-sliderX_->minimum()) / (sliderX_->maximum()-sliderX_->minimum());
    vec2 valueVec2 = property_->get();
    valueVec2.x = valuef;
    property_->set(valueVec2);
}

void FloatVec2PropertyWidgetQt::setYValue(int value) {
    float valuef = (static_cast<float>(value)-sliderY_->minimum()) / (sliderY_->maximum()-sliderY_->minimum());
    vec2 valueVec2 = property_->get();
    valueVec2.y = valuef;
    property_->set(valueVec2);
}

void FloatVec2PropertyWidgetQt::setPropertyValue() {
	setXValue(sliderX_->value());
	setYValue(sliderX_->value());
}

void FloatVec2PropertyWidgetQt::updateFromProperty() {
	vec2 valueVec2 = property_->get();
    int valueX = sliderX_->minimum() + static_cast<int>(ceilf(valueVec2.x * (sliderX_->maximum()-sliderX_->minimum())));
    sliderX_->setValue(valueX);
    int valueY = sliderY_->minimum() + static_cast<int>(ceilf(valueVec2.y * (sliderY_->maximum()-sliderY_->minimum())));
    sliderY_->setValue(valueY);
};


} //namespace