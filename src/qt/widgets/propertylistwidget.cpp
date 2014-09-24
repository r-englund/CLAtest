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
#include <inviwo/core/util/clock.h>
#include <inviwo/qt/widgets/properties/collapsiblegroupboxwidgetqt.h>
#include <QLabel>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <QSettings>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>

namespace inviwo {


PropertyListFrame::PropertyListFrame(QWidget* parent) : QWidget(parent) {
    QSizePolicy sp(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    sp.setVerticalStretch(0);
    sp.setHorizontalStretch(1);
    QWidget::setSizePolicy(sp);
}

QSize PropertyListFrame::sizeHint() const {
    QSize size = layout()->minimumSize();
    size.setHeight(parentWidget()->width());
    return size;
}

QSize PropertyListFrame::minimumSizeHint() const {
    QSize size = layout()->minimumSize();
    size.setWidth(parentWidget()->width());
    return size;
}

void PropertyListFrame::paintEvent(QPaintEvent*) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

PropertyListWidget::PropertyListWidget(QWidget* parent)
    : InviwoDockWidget(tr("Properties"), parent), PropertyListWidgetObservable() {
    setObjectName("ProcessorListWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    
    QSizePolicy sp(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
    sp.setVerticalStretch(1);
    sp.setHorizontalStretch(1);
    setSizePolicy(sp);
    
    scrollArea_ = new QScrollArea(this);
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setMinimumWidth(320);
    scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea_->setFrameShape(QFrame::NoFrame);
    scrollArea_->setContentsMargins(0, 0, 0, 0);

    listWidget_ = new PropertyListFrame(this);
    listLayout_ = new QVBoxLayout();
    listWidget_->setLayout(listLayout_);
    listLayout_->setAlignment(Qt::AlignTop);
    listLayout_->setContentsMargins(7, 7, 7, 7);
    listLayout_->setSpacing(7);
    listLayout_->setSizeConstraint(QLayout::SetMinAndMaxSize);

    scrollArea_->setWidget(listWidget_);
    setWidget(scrollArea_);

    usageMode_ = InviwoApplication::getPtr()->getSettingsByType<SystemSettings>()
                                            ->getApplicationUsageMode();
}

PropertyListWidget::~PropertyListWidget() {}

void PropertyListWidget::addProcessorProperties(Processor* processor) {
    CollapsibleGroupBoxWidgetQt* widget = getProcessorPropertiesItem(processor);

    if (widget) {
        WidgetMap::iterator elm = devWidgets_.find(processor);
        if (elm == devWidgets_.end()) {
            devWidgets_[processor] = widget;
        }
        IVW_CPU_PROFILING("show props" + processor->getIdentifier());
        widget->showWidget();
    }
    // Put this tab in front
    QWidget::raise();
}

void PropertyListWidget::removeProcessorProperties(Processor* processor) {
    WidgetMap::iterator it = widgetMap_.find(processor);

    if (it != widgetMap_.end()) {
        WidgetMap::iterator elm = devWidgets_.find(processor);
        if (elm != devWidgets_.end()) {
            devWidgets_.erase(elm);
        }
       
        it->second->hideWidget();
    }
}

void PropertyListWidget::removeAndDeleteProcessorProperties(Processor* processor) {
    WidgetMap::iterator it = widgetMap_.find(processor);

    if (it != widgetMap_.end()) {

        WidgetMap::iterator elm = devWidgets_.find(processor);
        if (elm != devWidgets_.end()) {
            devWidgets_.erase(elm);
        }

        it->second->hideWidget();
        listLayout_->removeWidget(it->second);

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
            delete propertyWidgets[i];
        }

        delete it->second;
        widgetMap_.erase(it);
    }
}

void PropertyListWidget::cacheProcessorPropertiesItem(Processor* processor) {
    getProcessorPropertiesItem(processor);
}

CollapsibleGroupBoxWidgetQt* PropertyListWidget::getProcessorPropertiesItem(Processor* processor) {
    
    CollapsibleGroupBoxWidgetQt* processorPropertyWidget = NULL;

    // check if processor widget has been already generated
    WidgetMap::iterator it = widgetMap_.find(processor);
    if (it != widgetMap_.end()) {
        processorPropertyWidget = it->second;
    } else {
        processorPropertyWidget = createNewProcessorPropertiesItem(processor);
    }

    return processorPropertyWidget;
}

CollapsibleGroupBoxWidgetQt* PropertyListWidget::createNewProcessorPropertiesItem(
    Processor* processor) {
    
    IVW_CPU_PROFILING("create props" + processor->getIdentifier());
    
    // create property widget and store it in the map
    CollapsibleGroupBoxWidgetQt* widget =
        new CollapsibleGroupBoxWidgetQt(processor->getIdentifier(), processor->getIdentifier());
    widget->hideWidget();
    
    std::vector<Property*> props = processor->getProperties();
    for (size_t i = 0; i < props.size(); i++) {
        widget->addProperty(props[i]);
    }

    listLayout_->insertWidget(0, widget, 0, Qt::AlignTop);
    widget->updateVisibility();
    widget->hideWidget();

    widgetMap_[processor] = widget;

    
    // add observer for onProcessorIdentifierChange
    processor->ProcessorObservable::addObserver(this);

    // Add the widget as a property owner observer for dynamic property addition and removal
    processor->PropertyOwnerObservable::addObserver(
        static_cast<PropertyOwnerObserver*>(widget));

    return widget;
}

void PropertyListWidget::propertyModified() { notifyPropertyListWidgetObservers(); }

void PropertyListWidget::setUsageMode(bool applicationMode) {
    if (applicationMode) {
        setUsageMode(APPLICATION);
    } else {
        setUsageMode(DEVELOPMENT);
    }
}

void PropertyListWidget::setUsageMode(UsageMode usageMode) {
    usageMode_ = usageMode;

    for (WidgetMap::const_iterator it = widgetMap_.begin(); it != widgetMap_.end(); it++) {
        CollapsibleGroupBoxWidgetQt* widget = it->second;

        widget->updateVisibility();

        if (usageMode_ == DEVELOPMENT) {
            widget->hideWidget();
        }

    }

    if (usageMode_ == DEVELOPMENT) {
        for (WidgetMap::iterator it = devWidgets_.begin(); it != devWidgets_.end(); ++it) {
            it->second->showWidget();
        }
    }
}

UsageMode PropertyListWidget::getUsageMode() {
    return usageMode_;
}

bool PropertyListWidget::event(QEvent* e) {
    // The network editor will post these events.
    if (e->type() == PropertyListEvent::type()) {
        PropertyListEvent* ple = static_cast<PropertyListEvent*>(e);
        ple->accept();

        Processor* p = ple->processor_;
        if (p == NULL) {
            return true;
        }

        switch (ple->action_) {
            case PropertyListEvent::ADD: {
                IVW_CPU_PROFILING("add props" + p->getIdentifier());
                addProcessorProperties(p);
                break;
            }
            case PropertyListEvent::REMOVE: {
                removeProcessorProperties(p);
                break;
            }
            case PropertyListEvent::CACHE: {
                IVW_CPU_PROFILING("cache props" + p->getIdentifier());
                cacheProcessorPropertiesItem(p);
                break;
            }
        }

        return true;
    } else {
        return QWidget::event(e);
    }
}

void PropertyListWidget::onProcessorIdentifierChange(Processor* processor) {
    WidgetMap::iterator it = widgetMap_.find(processor);
    CollapsibleGroupBoxWidgetQt* processorPropertyWidget = 0;

    if (it != widgetMap_.end()) {
        processorPropertyWidget = it->second;
        processorPropertyWidget->setIdentifier(processor->getIdentifier());
    }
}

void PropertyListWidget::updateProcessorIdentifier(std::string oldName, std::string newName) {
    for(WidgetMap::iterator it = widgetMap_.begin(); it!=widgetMap_.end(); ++it) {
        std::string processorName = it->first->getIdentifier();
        if (processorName == oldName) {
            it->first->setIdentifier(newName);
        }
    }
}

QEvent::Type PropertyListEvent::PROPERY_LIST_EVENT = QEvent::None;

}  // namespace