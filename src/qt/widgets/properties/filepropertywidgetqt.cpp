#include "inviwo/qt/widgets/properties/filepropertywidgetqt.h"

namespace inviwo {

FilePropertyWidgetQt::FilePropertyWidgetQt(FileProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void FilePropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    lineEdit_ = new QLineEdit();
    connect(lineEdit_, SIGNAL(returnPressed()), this, SLOT(setPropertyValue()));
    hLayout->addWidget(lineEdit_);
    setLayout(hLayout);
}

void FilePropertyWidgetQt::setPropertyValue() {
    property_->set(lineEdit_->text().toStdString());
}

void FilePropertyWidgetQt::updateFromProperty() {
    lineEdit_->setText(QString::fromStdString(property_->get()));
};

} // namespace
