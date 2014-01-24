/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

#include <inviwo/qt/widgets/propertylistwidget.h>
#include <inviwo/core/properties/propertywidgetfactory.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/settings/systemsettings.h>
#include <QLabel>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <QSettings>


namespace inviwo {

PropertyListWidget* PropertyListWidget::propertyListWidget_ = 0;

PropertyListWidget::PropertyListWidget(QWidget* parent) : InviwoDockWidget(tr("Properties"), parent), VoidObservable(){
    setObjectName("ProcessorListWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    propertyListWidget_ = this;

    scrollArea_ = new QScrollArea(propertyListWidget_);
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setMinimumWidth(300);
    scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea_->setFrameShape(QFrame::NoFrame);

    listWidget_ = new QWidget();
    listWidgetLayout_ = new QVBoxLayout(listWidget_);
    listWidgetLayout_->setAlignment(Qt::AlignTop);

    scrollArea_->setWidget(listWidget_);
    setWidget(scrollArea_);

    QSettings settings("Inviwo", "Inviwo");
    settings.beginGroup("PropertyListwidget");
    developerViewMode_ = settings.value("developerViewMode",true).toBool();
    applicationViewMode_ = settings.value("applicationViewMode",false).toBool();
    settings.endGroup();
}

PropertyListWidget::~PropertyListWidget() {}

void PropertyListWidget::addProcessorProperties(Processor* processor) {
    QWidget* processorPropertyWidget = getProcessorPropertiesItem(processor);
    if (processorPropertyWidget) {
        listWidgetLayout_->addWidget(processorPropertyWidget);
        processorPropertyWidget->setVisible(true);
        static_cast<CollapsibleGroupBoxWidgetQt*>(processorPropertyWidget)->show();
     }
}

void PropertyListWidget::removeAndDeleteProcessorProperties(Processor* processor) {
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(processor->getIdentifier());
    if (it != propertyWidgetMap_.end()) {
        it->second->setVisible(false);
        listWidgetLayout_->removeWidget(it->second);
        it->second->deleteLater();
        propertyWidgetMap_.erase(it);
    }   
}    

void PropertyListWidget::removeProcessorProperties(Processor* processor) {
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(processor->getIdentifier());
    if (it != propertyWidgetMap_.end()) {
        it->second->setVisible(false);
        listWidgetLayout_->removeWidget(it->second);
    }
}

void PropertyListWidget::changeName(std::string oldName, std::string newName) {
    // check if processor widget exists
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(oldName);
    if (it != propertyWidgetMap_.end()) {
        CollapsibleGroupBoxWidgetQt* processorPropertyWidget = dynamic_cast<CollapsibleGroupBoxWidgetQt*>(it->second);
        processorPropertyWidget->setIdentifier(newName);
        const_cast<std::string &>(it->first) = newName;
    }
}

void PropertyListWidget::cacheProcessorPropertiesItem(Processor* processor){
    getProcessorPropertiesItem(processor);
}

QWidget* PropertyListWidget::getProcessorPropertiesItem(Processor* processor){
    // check if processor widget has been already generated
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(processor->getIdentifier());
    QWidget* processorPropertyWidget = 0;
    if (it != propertyWidgetMap_.end()) {
        // property widget has already been created and stored in the map
        processorPropertyWidget = it->second;
    } else {
        processorPropertyWidget = createNewProcessorPropertiesItem(processor); 
    }
    return processorPropertyWidget;
}

QWidget* PropertyListWidget::createNewProcessorPropertiesItem(Processor* processor) {
    // create property widget and store it in the map
    CollapsibleGroupBoxWidgetQt* processorPropertyWidget = new CollapsibleGroupBoxWidgetQt(processor->getIdentifier(), processor->getIdentifier());
    processorPropertyWidget->setParent(this);

    properties_ = processor->getProperties();
    std::vector<Property*> addedProperties;
    for (size_t i=0; i<properties_.size(); i++) {
        Property* curProperty = properties_[i];
        // check if the property is already added
        if(std::find(addedProperties.begin(),addedProperties.end(),curProperty) != addedProperties.end())
            continue;
        // add to group box if one is assigned to the property
        else if (curProperty->getGroupID() != "") {
            CollapsibleGroupBoxWidgetQt* group = new CollapsibleGroupBoxWidgetQt(curProperty->getGroupID(), curProperty->getGroupDisplayName());
            group->setIdentifier(curProperty->getGroupDisplayName());
            // add all the properties with the same group assigned
            for (size_t k=0; k<properties_.size(); k++){
                Property* tmpProperty = properties_[k];
                if (curProperty->getGroupID() == tmpProperty->getGroupID()) {
                    group->addProperty(tmpProperty);
                    addedProperties.push_back(tmpProperty);
                }
            }
            group->generatePropertyWidgets();
            processorPropertyWidget->addWidget(group);
        }
        else {
            PropertyWidgetQt* propertyWidget =
            static_cast<PropertyWidgetQt*>(PropertyWidgetFactory::getRef().create(curProperty));
            if (propertyWidget) {          
                curProperty->registerWidget(propertyWidget);
                processorPropertyWidget->addWidget(propertyWidget);
                connect(propertyWidget, SIGNAL(modified()), this, SLOT(propertyModified()));
                addedProperties.push_back(curProperty);
            }
        }
    } 
    propertyWidgetMap_.insert(std::make_pair(processor->getIdentifier(), processorPropertyWidget));
    return processorPropertyWidget;
}

void PropertyListWidget::propertyModified() {
    notifyObservers();
}

PropertyListWidget* PropertyListWidget::instance() {
    return propertyListWidget_;
}

void PropertyListWidget::setDeveloperViewMode( bool value  ){
    if (value) {
        setViewMode(DEVELOPMENT);
    }
}

void PropertyListWidget::setApplicationViewMode(bool value){
    if (value) { 
        setViewMode(APPLICATION);
    }
}

void PropertyListWidget::setViewMode(PropertyVisibilityMode viewMode){
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    SystemSettings* mainSettings = inviwoApp->getSettingsByType<SystemSettings>();
    dynamic_cast<OptionPropertyInt*>(mainSettings->getPropertyByIdentifier("viewMode"))->set(static_cast<int>(viewMode));    

    applicationViewMode_ = (viewMode == APPLICATION);
    developerViewMode_ = (viewMode == DEVELOPMENT);
    for (size_t i = 0; i < properties_.size(); i++ ) {
        properties_[i]->updateVisibility();
    }
}

void PropertyListWidget::saveState(){
    QSettings settings("Inviwo", "Inviwo");
    settings.beginGroup("PropertyListwidget");
    settings.setValue("developerViewMode", developerViewMode_);
    settings.setValue("applicationViewMode",applicationViewMode_);
    settings.endGroup();
}

PropertyVisibilityMode PropertyListWidget::getVisibilityMode(){
    if (developerViewMode_) 
        return DEVELOPMENT;
    else
        return APPLICATION;
}

void PropertyListWidget::notify() {}

} // namespace