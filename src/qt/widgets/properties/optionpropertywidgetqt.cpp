#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <QComboBox>
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
   int size_ = static_cast<TemplatedOptionProperty<std::string>*>(property_)->getOptions().size();
   for (int i=0; i<size_; i++)
   {
       comboBox_->addItem(QString::fromStdString(static_cast<TemplatedOptionProperty<std::string>*>(property_)->getOptions().at(i).first));
   }   
}
void OptionPropertyWidgetQt::optionChanged(){
    std::cout<<static_cast<TemplatedOptionProperty<std::string>*>(property_)->getOptions().at(comboBox_->currentIndex()).second << std::endl;
    static_cast<TemplatedOptionProperty<std::string>*>(property_)->setSelectedOption(comboBox_->currentIndex());
}

void OptionPropertyWidgetQt::updateFromProperty() {
    comboBox_->setCurrentIndex(static_cast<TemplatedOptionProperty<std::string>*>(property_)->getSelectedOption());
}

} // namespace
