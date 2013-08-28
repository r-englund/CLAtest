#include <inviwo/qt/widgets/properties/compositepropertywidgetqt.h>

#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>

namespace inviwo {

CompositePropertyWidgetQt::CompositePropertyWidgetQt(CompositeProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void CompositePropertyWidgetQt::generateWidget() {
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

void CompositePropertyWidgetQt::updateFromProperty() {
    for (size_t i=0; i<subPropertyWidgets_.size(); i++)
        subPropertyWidgets_[i]->updateFromProperty();
}

void CompositePropertyWidgetQt::propertyModified() {
    emit modified();
}

} // namespace
