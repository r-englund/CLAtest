#include <inviwo/qt/widgets/properties/boolpropertywidgetqt.h>

namespace inviwo {

BoolPropertyWidgetQt::BoolPropertyWidgetQt(BoolProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void BoolPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    label_ = new EditableLabelQt(property_->getDisplayName());
    hLayout->addWidget(label_);
    checkBox_ = new QCheckBox();
    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    connect(checkBox_, SIGNAL(clicked()), this, SLOT(setPropertyValue()));
    hLayout->addWidget(checkBox_);
    setLayout(hLayout);
}

void BoolPropertyWidgetQt::setPropertyValue() {
    property_->set(checkBox_->isChecked());
    emit modified();
}

void BoolPropertyWidgetQt::updateFromProperty() {
    checkBox_->setChecked(property_->get());
}

void BoolPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}

} // namespace
