#include "inviwo/qt/widgets/properties/floatvec4propertywidgetqt.h"

#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

FloatVec4PropertyWidgetQt::FloatVec4PropertyWidgetQt(FloatVec4Property* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void FloatVec4PropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));

    QWidget* sliderWidget = new QWidget();
    QVBoxLayout* vLayout = new QVBoxLayout();
    sliderWidget->setLayout(vLayout);
    sliderX_ = new QSlider();
    sliderX_->setOrientation(Qt::Horizontal);
    vLayout->addWidget(sliderX_);
    sliderY_ = new QSlider(Qt::Horizontal);
    vLayout->addWidget(sliderY_);
    sliderZ_ = new QSlider(Qt::Horizontal);
    vLayout->addWidget(sliderZ_);
	sliderW_ = new QSlider(Qt::Horizontal);
    vLayout->addWidget(sliderW_);
    hLayout->addWidget(sliderWidget);
    connect(sliderX_, SIGNAL(valueChanged(int)), this, SLOT(setXValue(int)));
    connect(sliderY_, SIGNAL(valueChanged(int)), this, SLOT(setYValue(int)));
    connect(sliderZ_, SIGNAL(valueChanged(int)), this, SLOT(setZValue(int)));
	connect(sliderW_, SIGNAL(valueChanged(int)), this, SLOT(setWValue(int)));
    setLayout(hLayout);
}

void FloatVec4PropertyWidgetQt::setXValue(int value) {
    float valuef = (static_cast<float>(value)-sliderX_->minimum()) / (sliderX_->maximum()-sliderX_->minimum());
    vec4 valueVec4 = property_->get();
    valueVec4.x = valuef;
    property_->set(valueVec4);
}

void FloatVec4PropertyWidgetQt::setYValue(int value) {
    float valuef = (static_cast<float>(value)-sliderY_->minimum()) / (sliderY_->maximum()-sliderY_->minimum());
    vec4 valueVec4 = property_->get();
    valueVec4.y = valuef;
    property_->set(valueVec4);
}

void FloatVec4PropertyWidgetQt::setZValue(int value) {
    float valuef = (static_cast<float>(value)-sliderZ_->minimum()) / (sliderZ_->maximum()-sliderZ_->minimum());
    vec4 valueVec4 = property_->get();
    valueVec4.z = valuef;
    property_->set(valueVec4);
}

void FloatVec4PropertyWidgetQt::setWValue(int value) {
    float valuef = (static_cast<float>(value)-sliderW_->minimum()) / (sliderW_->maximum()-sliderW_->minimum());
    vec4 valueVec4 = property_->get();
    valueVec4.w = valuef;
    property_->set(valueVec4);
}

void FloatVec4PropertyWidgetQt::setPropertyValue() {
    setXValue(sliderX_->value());
    setYValue(sliderY_->value());
    setZValue(sliderZ_->value());
	setWValue(sliderW_->value());
}

void FloatVec4PropertyWidgetQt::updateFromProperty() {
    vec4 valueVec4 = property_->get();
    int valueX = sliderX_->minimum() + static_cast<int>(ceilf(valueVec4.x * (sliderX_->maximum()-sliderX_->minimum())));
    sliderX_->setValue(valueX);
    int valueY = sliderY_->minimum() + static_cast<int>(ceilf(valueVec4.y * (sliderY_->maximum()-sliderY_->minimum())));
    sliderY_->setValue(valueY);
    int valueZ = sliderZ_->minimum() + static_cast<int>(ceilf(valueVec4.z * (sliderZ_->maximum()-sliderZ_->minimum())));
    sliderZ_->setValue(valueZ);
	int valueW = sliderW_->minimum() + static_cast<int>(ceilf(valueVec4.w * (sliderW_->maximum()-sliderW_->minimum())));
    sliderW_->setValue(valueW);
};

} // namespace