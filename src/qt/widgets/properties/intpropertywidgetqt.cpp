#include <inviwo/qt/widgets/properties/intpropertywidgetqt.h>

#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

IntPropertyWidgetQt::IntPropertyWidgetQt(IntProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void IntPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    sliderWidget_  = new IntSliderWidgetQt(property_->getMinValue(), property_->getMaxValue(), property_->getIncrement());
    connect(sliderWidget_->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(setPropertyValueFromSlider()));
    connect(sliderWidget_->getSpinBox(), SIGNAL(valueChanged(int)),this,SLOT(setPropertyValueFromSpinBox()));
    hLayout->addWidget(sliderWidget_);
    setLayout(hLayout);
}



void IntPropertyWidgetQt::updateFromProperty() {
    int value = property_->get();
    sliderWidget_->setValue(value);
};

void IntPropertyWidgetQt::setPropertyValueFromSpinBox() {
    sliderWidget_->updateValueSlider();
    property_->set(sliderWidget_->getValue());

}

void IntPropertyWidgetQt::setPropertyValueFromSlider() {
    sliderWidget_->updateValueSpinbox();
    property_->set(sliderWidget_->getValue());

}

} // namespace
