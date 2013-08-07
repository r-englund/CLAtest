#include <inviwo/qt/widgets/properties/compositepropertywidgetqt.h>

#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>

namespace inviwo {

CompositePropertyWidgetQt::CompositePropertyWidgetQt(CompositeProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void CompositePropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    QFrame* frame = new QFrame();
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    frame->setLineWidth(2);
    hLayout->addWidget(frame);

    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    std::vector<Property*> subProperties = property_->getSubProperties();
    PropertyWidgetFactoryQt* propertyWidgetFactory = new PropertyWidgetFactoryQt();
    for (size_t i=0; i<subProperties.size(); i++) {
        Property* curProperty = subProperties[i];
        PropertyWidgetQt* propertyWidget = propertyWidgetFactory->create(curProperty);
        subPropertyWidgets_.push_back(propertyWidget);
        vLayout->addWidget(propertyWidget);
        curProperty->registerPropertyWidget(propertyWidget);
        connect(propertyWidget, SIGNAL(modified()), this, SLOT(propertyModified()));
    } 
    frame->setLayout(vLayout);
    setLayout(hLayout);
}

void CompositePropertyWidgetQt::updateFromProperty() {
    for (size_t i=0; i<subPropertyWidgets_.size(); i++)
        subPropertyWidgets_[i]->updateFromProperty();
}

void CompositePropertyWidgetQt::propertyModified() {
    emit modified();
}

} // namespace
