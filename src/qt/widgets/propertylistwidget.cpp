/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Timo Ropinski, Alexander Johansson, Erik Sundén
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/propertylistwidget.h>
#include <inviwo/core/properties/propertywidgetfactory.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/settings/systemsettings.h>
#include <inviwo/qt/widgets/properties/collapsiblegroupboxwidgetqt.h>
#include <QLabel>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <QSettings>

namespace inviwo {

PropertyListWidget* PropertyListWidget::propertyListWidget_ = 0;

PropertyListWidget::PropertyListWidget(QWidget* parent)
    : InviwoDockWidget(tr("Properties"), parent), PropertyListWidgetObservable() {
    setObjectName("ProcessorListWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    propertyListWidget_ = this;
    scrollArea_ = new QScrollArea(propertyListWidget_);
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setMinimumWidth(300);
    scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea_->setFrameShape(QFrame::NoFrame);
    scrollArea_->setContentsMargins(0, 0, 0, 0);

    listWidget_ = new QWidget();
    listLayout_ = new QVBoxLayout(listWidget_);
    listLayout_->setAlignment(Qt::AlignTop);
    listLayout_->setContentsMargins(7, 7, 7, 7);
    listLayout_->setSpacing(7);

    scrollArea_->setWidget(listWidget_);
    setWidget(scrollArea_);
    QSettings settings("Inviwo", "Inviwo");
    settings.beginGroup("PropertyListwidget");
    developerViewMode_ = settings.value("developerViewMode", true).toBool();
    applicationViewMode_ = settings.value("applicationViewMode", false).toBool();
    settings.endGroup();
}

PropertyListWidget::~PropertyListWidget() {}

void PropertyListWidget::addProcessorProperties(Processor* processor) {
    CollapsibleGroupBoxWidgetQt* processorPropertyWidget = getProcessorPropertiesItem(processor);

    if (processorPropertyWidget) {
        listLayout_->addWidget(processorPropertyWidget);
        processorPropertyWidget->setVisible(true);
        processorPropertyWidget->show();
        devWidgets_.push_back(processorPropertyWidget);
    }
    // Put this tab in front
    QWidget::raise();
}

void PropertyListWidget::removeProcessorProperties(Processor* processor) {
    WidgetMap::iterator it = widgetMap_.find(processor->getIdentifier());

    if (it != widgetMap_.end()) {
        it->second->setVisible(false);
        listLayout_->removeWidget(it->second);

        std::vector<CollapsibleGroupBoxWidgetQt*>::iterator elm =
            std::find(devWidgets_.begin(), devWidgets_.end(), it->second);
        if (elm != devWidgets_.end()) {
            devWidgets_.erase(elm);
        }
    }
}

void PropertyListWidget::removeAndDeleteProcessorProperties(Processor* processor) {
    WidgetMap::iterator it = widgetMap_.find(processor->getIdentifier());

    if (it != widgetMap_.end()) {
        it->second->setVisible(false);
        listLayout_->removeWidget(it->second);

        std::vector<CollapsibleGroupBoxWidgetQt*>::iterator elm =
            std::find(devWidgets_.begin(), devWidgets_.end(), it->second);
        if (elm != devWidgets_.end()) {
            devWidgets_.erase(elm);
        }

        CollapsibleGroupBoxWidgetQt* collapsiveGropWidget = it->second;
        std::vector<PropertyWidgetQt*> propertyWidgets = collapsiveGropWidget->getPropertyWidgets();
        std::vector<Property*> properties = processor->getProperties();

        for (size_t i = 0; i < propertyWidgets.size(); i++) {
            for (size_t j = 0; j < properties.size(); j++)
                properties[j]->deregisterWidget(propertyWidgets[i]);
        }

        for (size_t i = 0; i < propertyWidgets.size(); i++) {
            collapsiveGropWidget->removeWidget(propertyWidgets[i]);
            propertyWidgets[i]->hide();
            // TODO: Do not use deleteLater(). Widgets need to be deleted instantly or use
            // deinitialize()
            // Reason: These are cached widgets which has children widgets.
            // deleteLater() keeps the children active for a while that causes invalidation of
            // properties.
            // Hence deleteLater() cannot be used
            delete propertyWidgets[i];
        }

        delete it->second;
        widgetMap_.erase(it);
    }
}

void PropertyListWidget::changeName(std::string oldName, std::string newName) {
    // check if processor widget exists
    WidgetMap::iterator it = widgetMap_.find(oldName);

    if (it != widgetMap_.end()) {
        CollapsibleGroupBoxWidgetQt* processorPropertyWidget = it->second;
        processorPropertyWidget->setIdentifier(newName);
        widgetMap_.erase(it);
        widgetMap_[newName] = processorPropertyWidget;
    }
}

void PropertyListWidget::cacheProcessorPropertiesItem(Processor* processor) {
    getProcessorPropertiesItem(processor);
}

CollapsibleGroupBoxWidgetQt* PropertyListWidget::getProcessorPropertiesItem(Processor* processor) {
    // check if processor widget has been already generated
    WidgetMap::iterator it = widgetMap_.find(processor->getIdentifier());
    CollapsibleGroupBoxWidgetQt* processorPropertyWidget = 0;

    if (it != widgetMap_.end()) {
        // property widget has already been created and stored in the map
        processorPropertyWidget = it->second;
    } else
        processorPropertyWidget = createNewProcessorPropertiesItem(processor);

    return processorPropertyWidget;
}

CollapsibleGroupBoxWidgetQt* PropertyListWidget::createNewProcessorPropertiesItem(
    Processor* processor) {
    // create property widget and store it in the map
    CollapsibleGroupBoxWidgetQt* processorPropertyWidget =
        new CollapsibleGroupBoxWidgetQt(processor->getIdentifier(), processor->getIdentifier());
    processorPropertyWidget->setParent(this);

    std::vector<Property*> props = processor->getProperties();
    WidgetMap groups;

    for (size_t i = 0; i < props.size(); i++) {
        if (props[i]->getGroupID() != "") {
            WidgetMap::iterator it = groups.find(props[i]->getGroupID());
            if (it == groups.end()) {
                CollapsibleGroupBoxWidgetQt* group = new CollapsibleGroupBoxWidgetQt(
                    props[i]->getGroupDisplayName(), props[i]->getGroupDisplayName());
                processorPropertyWidget->addWidget(group);
                groups.insert(std::make_pair(props[i]->getGroupID(), group));
                group->addProperty(props[i]);
            } else {
                it->second->addProperty(props[i]);
            }
        } else {
            processorPropertyWidget->addProperty(props[i]);
        }
    }

    widgetMap_.insert(std::make_pair(processor->getIdentifier(), processorPropertyWidget));
    return processorPropertyWidget;
}

void PropertyListWidget::propertyModified() { notifyPropertyListWidgetObservers(); }

PropertyListWidget* PropertyListWidget::instance() { return propertyListWidget_; }

void PropertyListWidget::setVisibilityMode(bool applicationView) {
    if (applicationView) {
        setVisibilityMode(APPLICATION);
    } else {
        setVisibilityMode(DEVELOPMENT);
    }
}

void PropertyListWidget::setVisibilityMode(PropertyVisibilityMode appVisibilityMode) {
    applicationViewMode_ = (appVisibilityMode == APPLICATION);
    developerViewMode_ = (appVisibilityMode == DEVELOPMENT);

    for (WidgetMap::const_iterator it = widgetMap_.begin(); it != widgetMap_.end(); it++) {
        CollapsibleGroupBoxWidgetQt* widget = it->second;

        widget->updateVisibility();

        if (appVisibilityMode == DEVELOPMENT) {
            listLayout_->removeWidget(widget);
            widget->setVisible(false);

        } else if (appVisibilityMode == APPLICATION) {
            listLayout_->addWidget(widget);
            if (widget->getVisibilityMode() <= APPLICATION) {
                widget->setVisible(true);
            } else {
                widget->setVisible(false);
            }
        }
    }

    if (appVisibilityMode == DEVELOPMENT) {
        for (std::vector<CollapsibleGroupBoxWidgetQt*>::iterator it = devWidgets_.begin();
             it != devWidgets_.end(); ++it) {
            CollapsibleGroupBoxWidgetQt* widget = *it;
            listLayout_->addWidget(widget);
            widget->setVisible(true);
        }
    }
}

void PropertyListWidget::saveState() {
    QSettings settings("Inviwo", "Inviwo");
    settings.beginGroup("PropertyListwidget");
    settings.setValue("developerViewMode", developerViewMode_);
    settings.setValue("applicationViewMode", applicationViewMode_);
    settings.endGroup();
}

PropertyVisibilityMode PropertyListWidget::getVisibilityMode() {
    if (developerViewMode_)
        return DEVELOPMENT;
    else
        return APPLICATION;
}

}  // namespace