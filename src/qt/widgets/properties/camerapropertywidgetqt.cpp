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
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    slider_ = new QSlider();
    slider_->setOrientation(Qt::Horizontal);
    connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(setPropertyValue()));
    hLayout->addWidget(slider_);
    setLayout(hLayout);
}

void CameraPropertyWidgetQt::setPropertyValue() {
}

void CameraPropertyWidgetQt::updateFromProperty() {
};

} // namespace
