#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <QComboBox>
namespace inviwo {

OptionPropertyWidgetQt::OptionPropertyWidgetQt(OptionProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void OptionPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    comboBox_ = new QComboBox();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    hLayout->addWidget(comboBox_);
    setLayout(hLayout);
}

void OptionPropertyWidgetQt::setPropertyValue() {
}

void OptionPropertyWidgetQt::updateFromProperty() {
}

} // namespace
