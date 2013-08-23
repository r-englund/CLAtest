#include <inviwo/qt/widgets/properties/stringpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/compositepropertywidgetqt.h>

namespace inviwo {

StringPropertyWidgetQt::StringPropertyWidgetQt(StringProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void StringPropertyWidgetQt::generateWidget() {
    setObjectName("StringPropertyWidgetQt");
    QHBoxLayout* hLayout = new QHBoxLayout();
    lineEdit_ = new QLineEdit;
    label_ = new EditableLabelQt(property_->getDisplayName());
    hLayout->addWidget(label_);
    connect(lineEdit_, SIGNAL(editingFinished()), this, SLOT(setPropertyValue()));
    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    hLayout->addWidget(lineEdit_);
    setLayout(hLayout);
}

void StringPropertyWidgetQt::setPropertyValue() {
    std::string valueStr = lineEdit_->text().toLocal8Bit().constData();
    property_->set(valueStr);
    emit modified();
}

void StringPropertyWidgetQt::updateFromProperty() {
   lineEdit_->setText(QString::fromStdString(property_->get()));

}

void StringPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}

} // namespace