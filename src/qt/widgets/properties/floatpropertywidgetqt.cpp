#include <inviwo/qt/widgets/properties/floatpropertywidgetqt.h>

namespace inviwo {

FloatPropertyWidgetQt::FloatPropertyWidgetQt(FloatProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void FloatPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    slider_ = new QSlider();
    slider_->setOrientation(Qt::Horizontal);
    connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(setPropertyValue()));
    hLayout->addWidget(slider_);
    setLayout(hLayout);
}

void FloatPropertyWidgetQt::setPropertyValue() {
    float valuef = (static_cast<float>(slider_->value())-slider_->minimum()) / (slider_->maximum()-slider_->minimum());
    property_->set(valuef);
}

void FloatPropertyWidgetQt::updateFromProperty() {
    float valuef = property_->get();
    int value = slider_->minimum() + static_cast<int>(ceilf(valuef * (slider_->maximum()-slider_->minimum())));
    slider_->setValue(value);
}

} // namespace
