#include <inviwo/qt/editor/propertylistwidget.h>
#include <inviwo/core/properties/propertywidgetfactory.h>

#include <QLabel>
#include <QVBoxLayout>


namespace inviwo {

PropertyListWidget* PropertyListWidget::propertyListWidget_ = 0;

PropertyListWidget::PropertyListWidget(QWidget* parent) : InviwoDockWidget(tr("Properties"), parent), VoidObservable() {
    setObjectName("ProcessorListWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    propertyListWidget_ = this;
    
    scrollArea_ = new QScrollArea(propertyListWidget_);
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setMinimumWidth(300);
    scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    listWidget_ = new QWidget();
    listWidgetLayout_ = new QVBoxLayout(listWidget_);
    listWidgetLayout_->setAlignment(Qt::AlignTop);

    scrollArea_->setWidget(listWidget_);
    setWidget(scrollArea_);
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

    if (processorPropertyWidget) {
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

    if (processorPropertyWidget) {
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
    vLayout->setSpacing(0);

    QLabel* processorLabel = new QLabel(QString::fromStdString(processor->getIdentifier()));
    processorLabel->setAlignment(Qt::AlignCenter);
    processorLabel->setAutoFillBackground(true);
    processorLabel->setFrameStyle(QFrame::StyledPanel);

    vLayout->addWidget(processorLabel);
    std::vector<Property*> properties = processor->getProperties();
    std::vector<Property*> addedProperties;
    for (size_t i=0; i<properties.size(); i++) {
        Property* curProperty = properties[i];
        // check if the property is already added
        if(std::find(addedProperties.begin(),addedProperties.end(),curProperty) != addedProperties.end())
            continue;
        // add to group box if one is assigned to the property
        else if (curProperty->getGroupID()!="") {
            CollapsiveGroupBoxWidgetQt* group = new CollapsiveGroupBoxWidgetQt(curProperty->getGroupID(),curProperty->getGroupDisplayName());
            // add all the properties with the same group assigned
            for (size_t k=0; k<properties.size(); k++){
                Property* tmpProperty = properties[k];
                if (curProperty->getGroupID() == tmpProperty->getGroupID()) {
                    group->addProperty(tmpProperty);
                    addedProperties.push_back(tmpProperty);
                }
            }
            group->generatePropertyWidgets();
            vLayout->addWidget(group);
        }
        else {
            PropertyWidgetQt* propertyWidget = PropertyWidgetFactoryQt::getRef().create(curProperty);
            if (propertyWidget) {
                vLayout->addWidget(propertyWidget);
                curProperty->registerPropertyWidget(propertyWidget);
                connect(propertyWidget, SIGNAL(modified()), this, SLOT(propertyModified()));
                addedProperties.push_back(curProperty);
            }
        }

    } 
    vLayout->addStretch(1);
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

void PropertyListWidget::propertyModified() {
    notifyObservers();
}

PropertyListWidget* PropertyListWidget::instance() {
    return propertyListWidget_;
}

} // namespace