#include <inviwo/qt/widgets/properties/camerapropertywidgetqt.h>

#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

CameraPropertyWidgetQt::CameraPropertyWidgetQt(CameraProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

//void CameraPropertyWidgetQt::generateWidget() {
//    QHBoxLayout* hLayout = new QHBoxLayout();
//    label_ = new EditableLabelQt(property_->getDisplayName());
//    hLayout->addWidget(label_);
//    slider_ = new QSlider();
//    slider_->setOrientation(Qt::Horizontal);
//    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
//    connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(setPropertyValue()));
//    hLayout->addWidget(slider_);
//    setLayout(hLayout);
//}

void CameraPropertyWidgetQt::generateWidget() {
    QVBoxLayout* vLayout = new QVBoxLayout();
    collapsiveGroupBoxWidget_ = new CollapsiveGroupBoxWidgetQt(property_->getIdentifier(),property_->getDisplayName());
    std::vector<Property*> subProperties = property_->getSubProperties();
    for (size_t i=0; i<subProperties.size(); i++) {
        Property* curProperty = subProperties[i];
        collapsiveGroupBoxWidget_->addProperty(curProperty);
    }
    collapsiveGroupBoxWidget_->generatePropertyWidgets();
    vLayout->addWidget(collapsiveGroupBoxWidget_);

    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    setLayout(vLayout);
}

void CameraPropertyWidgetQt::setPropertyValue() {
}

void CameraPropertyWidgetQt::updateFromProperty() {
};

void CameraPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}

void CameraPropertyWidgetQt::visibilityModified( int mode ){
    property_->setVisibility(static_cast<PropertyVisibilityMode>(mode));
    std::vector<Property*> subProperties = property_->getSubProperties();
    for (size_t i=0; i<subProperties.size(); i++)
        subProperties[i]->setVisibility(static_cast<PropertyVisibilityMode>(mode));

}

} // namespace
