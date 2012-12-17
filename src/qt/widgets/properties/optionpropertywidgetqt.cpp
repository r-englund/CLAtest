#include "inviwo/qt/widgets/properties/optionpropertywidgetqt.h"

namespace inviwo {

OptionPropertyWidgetQt::OptionPropertyWidgetQt(OptionProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void OptionPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    setLayout(hLayout);
}

void OptionPropertyWidgetQt::setPropertyValue() {
}

void OptionPropertyWidgetQt::updateFromProperty() {
}

} // namespace
