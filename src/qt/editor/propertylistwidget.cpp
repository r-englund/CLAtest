#include "inviwo/qt/editor/propertylistwidget.h"

#include <QLabel>
#include <QVBoxLayout>

namespace inviwo {

PropertyListWidget* PropertyListWidget::propertyListWidget_ = 0;

PropertyListWidget::PropertyListWidget(QWidget* parent) : InviwoDockWidget(tr("Properties"), parent) {
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setMinimumWidth(200);
    propertyListWidget_ = this;
    propertyWidgetFactory_ = new PropertyWidgetFactoryQt();
}

PropertyListWidget::~PropertyListWidget() {}

void PropertyListWidget::showProcessorProperties(Processor* processor) {
    // check if processor widgets are already generated
    // if not generate and store them
    // else obtain them
    // show them
    QWidget* processorPropertyWidget = new QWidget();
    QVBoxLayout* vLayout = new QVBoxLayout(processorPropertyWidget);
    vLayout->addWidget(new QLabel(QString::fromStdString(processor->getIdentifier())));
    std::vector<Property*> properties = processor->getProperties();
    for (size_t i=0; i<properties.size(); i++) {
        Property* curProperty = properties[i];
        PropertyWidgetQt* propertyWidget = propertyWidgetFactory_->create(curProperty);
        vLayout->addWidget(propertyWidget);
        curProperty->registerPropertyWidget(propertyWidget);
    }
    setWidget(processorPropertyWidget);
}

PropertyListWidget* PropertyListWidget::instance() {
    return propertyListWidget_;
}

} // namespace