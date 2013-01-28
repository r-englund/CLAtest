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
    slider_ = new QSlider();
    slider_->setOrientation(Qt::Horizontal);
    connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(setPropertyValue()));
    hLayout->addWidget(slider_);
    setLayout(hLayout);
}

void IntPropertyWidgetQt::setPropertyValue() {
    property_->set(slider_->value());
}

void IntPropertyWidgetQt::updateFromProperty() {
    int value = property_->get();
    slider_->setValue(value);
};

} // namespace
