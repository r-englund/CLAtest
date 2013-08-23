#include <inviwo/qt/widgets/properties/camerapropertywidgetqt.h>

#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

CameraPropertyWidgetQt::CameraPropertyWidgetQt(CameraProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void CameraPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    label_ = new EditableLabelQt(property_->getDisplayName());
    hLayout->addWidget(label_);
    slider_ = new QSlider();
    slider_->setOrientation(Qt::Horizontal);
    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(setPropertyValue()));
    hLayout->addWidget(slider_);
    setLayout(hLayout);
}

void CameraPropertyWidgetQt::setPropertyValue() {
}

void CameraPropertyWidgetQt::updateFromProperty() {
};

void CameraPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}

} // namespace
