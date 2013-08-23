#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <QComboBox>
#include <typeinfo>
namespace inviwo {
OptionPropertyWidgetQt::OptionPropertyWidgetQt(BaseOptionProperty* property) : property_(property) { 
    generateWidget();
    updateFromProperty();
}

void OptionPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    comboBox_ = new QComboBox();
    fillComboBox();
    updateFromProperty();
    label_ = new EditableLabelQt(property_->getDisplayName());
    hLayout->addWidget(label_);
    hLayout->addWidget(comboBox_);
    setLayout(hLayout);
    connect(comboBox_, SIGNAL(currentIndexChanged(int)),this, SLOT(optionChanged()));
    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
}

void OptionPropertyWidgetQt::fillComboBox() {
    for (int i=0; i<property_->numOptions(); i++) {
        comboBox_->addItem(QString::fromStdString(property_->getOptionDisplayNames()[i]));
    }
}
void OptionPropertyWidgetQt::optionChanged() {
    property_->setSelectedOption(comboBox_->currentIndex());
    emit modified();
}

void OptionPropertyWidgetQt::updateFromProperty() {
    int index = property_->getSelectedOption();
    comboBox_->setCurrentIndex(index);
}

void OptionPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}

} // namespace
