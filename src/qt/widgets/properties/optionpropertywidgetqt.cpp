//#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
//#include <QComboBox>
//namespace inviwo {
//
//OptionPropertyWidgetQt::OptionPropertyWidgetQt(StringOptionProperty* property) : property_(property) {
//    generateWidget();
//    updateFromProperty();
//}
//
//void OptionPropertyWidgetQt::generateWidget() {
//    QHBoxLayout* hLayout = new QHBoxLayout();
//    comboBox_ = new QComboBox();
//    fillComboBox();
//    //hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
//    hLayout->addWidget(comboBox_);
//    setLayout(hLayout);
//}
//
//void OptionPropertyWidgetQt::fillComboBox(){
//   //int size_ = property_->getOptions().size();
//   //for (int i; i<size_; i++)
//   //{
//   //    comboBox_->addItem(QString::fromStdString(property_->getOptions().at(i).first));
//   //}
//    
//}
////template<typename T>
////void OptionPropertyWidgetQt<T>::setPropertyValue() {}
//
//void OptionPropertyWidgetQt::updateFromProperty() {}
//
//} // namespace
