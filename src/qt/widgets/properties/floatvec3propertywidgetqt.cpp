#include "inviwo/qt/widgets/properties/floatvec3propertywidgetqt.h"

#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

FloatVec3PropertyWidgetQt::FloatVec3PropertyWidgetQt(FloatVec3Property* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void FloatVec3PropertyWidgetQt::generateWidget() {
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
    hLayout->addWidget(sliderWidget);
    connect(sliderX_, SIGNAL(valueChanged(int)), this, SLOT(setXValue(int)));
    connect(sliderY_, SIGNAL(valueChanged(int)), this, SLOT(setYValue(int)));
    connect(sliderZ_, SIGNAL(valueChanged(int)), this, SLOT(setZValue(int)));
    setLayout(hLayout);
}

void FloatVec3PropertyWidgetQt::setXValue(int value) {
    float valuef = (static_cast<float>(value)-sliderX_->minimum()) / (sliderX_->maximum()-sliderX_->minimum());
    vec3 valueVec3 = property_->get();
    valueVec3.x = valuef;
    property_->set(valueVec3);
}

void FloatVec3PropertyWidgetQt::setYValue(int value) {
    float valuef = (static_cast<float>(value)-sliderY_->minimum()) / (sliderY_->maximum()-sliderY_->minimum());
    vec3 valueVec3 = property_->get();
    valueVec3.y = valuef;
    property_->set(valueVec3);
}

void FloatVec3PropertyWidgetQt::setZValue(int value) {
    float valuef = (static_cast<float>(value)-sliderZ_->minimum()) / (sliderZ_->maximum()-sliderZ_->minimum());
    vec3 valueVec3 = property_->get();
    valueVec3.z = valuef;
    property_->set(valueVec3);
}

void FloatVec3PropertyWidgetQt::updateFromProperty() {
    vec3 valueVec3 = property_->get();
    int valueX = sliderX_->minimum() + static_cast<int>(ceilf(valueVec3.x * (sliderX_->maximum()-sliderX_->minimum())));
    sliderX_->setValue(valueX);
    int valueY = sliderY_->minimum() + static_cast<int>(ceilf(valueVec3.y * (sliderY_->maximum()-sliderY_->minimum())));
    sliderY_->setValue(valueY);
    int valueZ = sliderZ_->minimum() + static_cast<int>(ceilf(valueVec3.z * (sliderZ_->maximum()-sliderZ_->minimum())));
    sliderZ_->setValue(valueZ);
};

} // namespace