#include "inviwo/qt/widgets/properties/boolpropertywidgetqt.h"

namespace inviwo {

BoolPropertyWidgetQt::BoolPropertyWidgetQt(BoolProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void BoolPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    checkBox_ = new QCheckBox();
    connect(checkBox_, SIGNAL(clicked()), this, SLOT(setPropertyValue()));
    hLayout->addWidget(checkBox_);
    setLayout(hLayout);
}

void BoolPropertyWidgetQt::setPropertyValue() {
    property_->set(checkBox_->isChecked());
}

void BoolPropertyWidgetQt::updateFromProperty() {
    checkBox_->setChecked(property_->get());
}

} // namespace
