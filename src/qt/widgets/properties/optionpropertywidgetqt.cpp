#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <QComboBox>
#include <typeinfo>
namespace inviwo {
OptionPropertyWidgetQt::OptionPropertyWidgetQt(BaseOptionProperty* property) : property_(property) { 
    setType();
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
    if (NULL != propertyDouble_){
        int size_ = propertyDouble_->getOptions().size();
        for(int i=0; i<size_;i++){
            comboBox_->addItem(QString::fromStdString(propertyDouble_->getOptions().at(i).first));
        }
        return;
    }
    if (NULL != propertyFloat_){
        int size_ = propertyFloat_->getOptions().size();
        for(int i=0; i<size_;i++){
            comboBox_->addItem(QString::fromStdString(propertyFloat_->getOptions().at(i).first));
        }
        return;
    }
    if (NULL != propertyString_)
    {
        int size_ = propertyString_->getOptions().size();
        for(int i=0; i<size_;i++){
            comboBox_->addItem(QString::fromStdString(propertyString_->getOptions().at(i).first));
        }
        return;
    }
    if (NULL != propertyInt_){
        int size_ = propertyInt_->getOptions().size();
        for(int i=0; i<size_;i++){
            comboBox_->addItem(QString::fromStdString(propertyInt_->getOptions().at(i).first));
        }
        return;
    }

}
void OptionPropertyWidgetQt::optionChanged(){
    if(NULL !=propertyDouble_){
        propertyDouble_->setSelectedOption(comboBox_->currentIndex());
        return;
    }
    if(NULL !=propertyFloat_){
        propertyFloat_->setSelectedOption(comboBox_->currentIndex());
        return;
    }
    if(NULL !=propertyString_){
        propertyString_->setSelectedOption(comboBox_->currentIndex());
        return;
    }
    if(NULL !=propertyInt_){
        propertyInt_->setSelectedOption(comboBox_->currentIndex());
        return;
    }

}

void OptionPropertyWidgetQt::updateFromProperty() {
    if(NULL !=propertyDouble_){
        comboBox_->setCurrentIndex(propertyDouble_->getSelectedOption());
        return;
    }
    if(NULL !=propertyFloat_){
        comboBox_->setCurrentIndex(propertyFloat_->getSelectedOption());
        return;
    }
    if(NULL !=propertyString_){
        comboBox_->setCurrentIndex(propertyString_->getSelectedOption());
        return;
    }
    if(NULL !=propertyInt_){
        comboBox_->setCurrentIndex(propertyInt_->getSelectedOption());
        return;
    }

}

void OptionPropertyWidgetQt::setType(){
   
    propertyDouble_= NULL;
    propertyFloat_= NULL;
    propertyInt_ = NULL;
    propertyString_ = NULL;
    
    if (typeid(*property_)==typeid(OptionPropertyDouble)){
        propertyDouble_=static_cast<OptionPropertyDouble*>(property_);
        return;
    }
    if (typeid(*property_)==typeid(OptionPropertyFloat)){
        propertyFloat_=static_cast<OptionPropertyFloat*>(property_);
        return;
    }
    if (typeid(*property_)==typeid(OptionPropertyInt)){
        propertyInt_=static_cast<OptionPropertyInt*>(property_);
        return;
    }
    if (typeid(*property_)==typeid(OptionPropertyString)){
        propertyString_=static_cast<OptionPropertyString*>(property_);
        return;
    }
}

} // namespace
