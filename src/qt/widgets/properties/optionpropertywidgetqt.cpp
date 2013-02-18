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
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    hLayout->addWidget(comboBox_);
    setLayout(hLayout);
    connect(comboBox_, SIGNAL(currentIndexChanged(int)),this, SLOT(optionChanged()));
    
}

void OptionPropertyWidgetQt::fillComboBox(){
    int size_ = property_->optionKeys_.size();
    for (int i=0; i < size_; i++){
        comboBox_->addItem(QString::fromStdString(property_->optionKeys_.at(i)));
    }
}
void OptionPropertyWidgetQt::optionChanged(){
    property_->setSelectedOption(comboBox_->currentIndex());
}

void OptionPropertyWidgetQt::updateFromProperty() {
    comboBox_->setCurrentIndex(property_->getSelectedOption());
}


} // namespace
