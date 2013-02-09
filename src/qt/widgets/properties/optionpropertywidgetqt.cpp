#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <QComboBox>
namespace inviwo {
template<typename T>
OptionPropertyWidgetQt<T>::OptionPropertyWidgetQt(TemplatedOptionProperty<T>* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}
template<typename T>
void OptionPropertyWidgetQt<T>::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    comboBox_ = new QComboBox();
    fillComboBox();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    hLayout->addWidget(comboBox_);
    setLayout(hLayout);
}
template<typename T>
void OptionPropertyWidgetQt<T>::fillComboBox(){
   int size_ = property_->getOptions().size();
   for (int i; i<size_; i++)
   {
       comboBox_->addItem(QString::fromStdString(property_->getOptions().at(i).first));
   }
    
}
//template<typename T>
//void OptionPropertyWidgetQt<T>::setPropertyValue() {}
template<typename T>
void OptionPropertyWidgetQt<T>::updateFromProperty() {}

} // namespace
