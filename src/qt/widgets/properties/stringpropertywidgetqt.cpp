#include <inviwo/qt/widgets/properties/stringpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/compositepropertywidgetqt.h>

namespace inviwo {

StringPropertyWidgetQt::StringPropertyWidgetQt(StringProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void StringPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    lineEdit_ = new QLineEdit;
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    connect(lineEdit_, SIGNAL(editingFinished()), this, SLOT(setPropertyValue()));
    hLayout->addWidget(lineEdit_);
    setLayout(hLayout);
}

void StringPropertyWidgetQt::setPropertyValue() {
    std::string valueStr = lineEdit_->text().toLocal8Bit().constData();
    property_->set(valueStr);
}

void StringPropertyWidgetQt::updateFromProperty() {
   lineEdit_->setText(QString::fromStdString(property_->get()));

}

} // namespace