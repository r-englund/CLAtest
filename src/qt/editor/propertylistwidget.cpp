#include "inviwo/qt/editor/propertylistwidget.h"
#include "inviwo/core/properties/propertywidgetfactory.h"

#include <QLabel>
#include <QVBoxLayout>

namespace inviwo {

PropertyListWidget* PropertyListWidget::propertyListWidget_ = 0;

PropertyListWidget::PropertyListWidget(QWidget* parent) : InviwoDockWidget(tr("Properties"), parent) {
    setObjectName("ProcessorListWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setMinimumWidth(300);
    propertyListWidget_ = this;

    listWidget_ = new QWidget(propertyListWidget_);
    listWidgetLayout_ = new QVBoxLayout(listWidget_);
    listWidgetLayout_->setAlignment(Qt::AlignTop);
    setWidget(listWidget_);
}

PropertyListWidget::~PropertyListWidget() {}

void PropertyListWidget::showProcessorProperties(std::vector<Processor*> processors) {
    for (size_t i=0; i<processors.size(); i++) {
        addProcessorPropertiesToLayout(processors[i]);
    }
}

void PropertyListWidget::showProcessorProperties(Processor* processor) {
    // check if processor widget has been already generated
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(processor->getIdentifier());
    QWidget* processorPropertyWidget = 0;
    if (it != propertyWidgetMap_.end()) {
        // property widget has already been created and stored in the map
        processorPropertyWidget = it->second;
    } else {
        processorPropertyWidget = createNewProcessorPropertiesItem(processor);
    }

    if(processorPropertyWidget) {
        removeAllProcessorProperties();
        processorPropertyWidget->setVisible(true);
        listWidgetLayout_->addWidget(processorPropertyWidget);
    }
}

void PropertyListWidget::addProcessorPropertiesToLayout(Processor* processor) {
    // check if processor widget has been already generated
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(processor->getIdentifier());
    QWidget* processorPropertyWidget = 0;
    if (it != propertyWidgetMap_.end()) {
        // property widget has already been created and stored in the map
        processorPropertyWidget = it->second;
    } else {
        processorPropertyWidget = createNewProcessorPropertiesItem(processor); 
    }

    if(processorPropertyWidget) {
        processorPropertyWidget->setVisible(true);
        listWidgetLayout_->addWidget(processorPropertyWidget);
    }
}

void PropertyListWidget::removeAllProcessorProperties() {
    while (QLayoutItem* item = listWidgetLayout_->takeAt(0)) {
        QWidget* processorPropertyWidget = item->widget();
        listWidgetLayout_->removeWidget(processorPropertyWidget);
        processorPropertyWidget->setVisible(false);
    }
}

QWidget* PropertyListWidget::createNewProcessorPropertiesItem(Processor* processor) {
    // create property widget and store it in the map
    QWidget* processorPropertyWidget = new QWidget(this, Qt::WindowStaysOnBottomHint);
    QVBoxLayout* vLayout = new QVBoxLayout(processorPropertyWidget);
    vLayout->setAlignment(Qt::AlignTop);

    QLabel* processorLabel = new QLabel(QString::fromStdString(processor->getIdentifier()));
    processorLabel->setAlignment(Qt::AlignCenter);
    processorLabel->setAutoFillBackground(true);
    processorLabel->setFrameStyle(QFrame::StyledPanel);

    vLayout->addWidget(processorLabel);
    std::vector<Property*> properties = processor->getProperties();
    for (size_t i=0; i<properties.size(); i++) {
        Property* curProperty = properties[i];
        PropertyWidgetQt* propertyWidget = PropertyWidgetFactoryQt::getRef().create(curProperty);
        vLayout->addWidget(propertyWidget);
        curProperty->registerPropertyWidget(propertyWidget);
    } 
    propertyWidgetMap_.insert(std::make_pair(processor->getIdentifier(), processorPropertyWidget));

    return processorPropertyWidget;
}

void PropertyListWidget::removeProcessorProperties(Processor* processor) {
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(processor->getIdentifier());
    if (it != propertyWidgetMap_.end()) { 
        QWidget* processorPropertyWidget = propertyWidgetMap_[processor->getIdentifier()];
        listWidgetLayout_->removeWidget(processorPropertyWidget);
        processorPropertyWidget->setVisible(false);
        propertyWidgetMap_.erase(processor->getIdentifier());
        delete processorPropertyWidget;
    }
}

PropertyListWidget* PropertyListWidget::instance() {
    return propertyListWidget_;
}

} // namespace