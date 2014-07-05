/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file author: Alexander Johansson
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/properties/collapsiblegroupboxwidgetqt.h>
#include <inviwo/qt/widgets/properties/compositepropertywidgetqt.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/settings/systemsettings.h>
#include <inviwo/core/properties/propertywidgetfactory.h>
#include <inviwo/core/network/processornetwork.h>

namespace inviwo {

CollapsibleGroupBoxWidgetQt::CollapsibleGroupBoxWidgetQt(std::string identifier,
                                                         std::string displayName)
    : PropertyWidgetQt(), identifier_(identifier), displayName_(displayName), collapsed_(false) {
    setObjectName("CollapsibleGroupBoxWidgetQt");
    generateWidget();
    updateFromProperty();
}

void CollapsibleGroupBoxWidgetQt::generateWidget() {
    propertyWidgetGroupLayout_ = new QVBoxLayout();
    propertyWidgetGroupLayout_->setAlignment(Qt::AlignTop);
    propertyWidgetGroupLayout_->setContentsMargins(
        PropertyWidgetQt::SPACING, PropertyWidgetQt::SPACING, PropertyWidgetQt::SPACING,
        PropertyWidgetQt::SPACING);
    propertyWidgetGroupLayout_->setSpacing(PropertyWidgetQt::SPACING);

    propertyWidgetGroup_ = new QWidget(this);
    propertyWidgetGroup_->setLayout(propertyWidgetGroupLayout_);

    btnCollapse_ = new QToolButton(this);
    btnCollapse_->setIcon(QIcon(":/stylesheets/images/arrow_darker_down.png"));
    connect(btnCollapse_, SIGNAL(clicked()), this, SLOT(toggleCollapsed()));

    label_ = new EditableLabelQt(this, displayName_, false);
    connect(label_, SIGNAL(textChanged()), this, SLOT(labelDidChange()));

    QToolButton* resetButton = new QToolButton(this);
    resetButton->setIcon(QIcon(":/icons/edit-undo-icon.png"));
    resetButton->setIconSize(QSize(20, 20));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetPropertyToDefaultState()));
    resetButton->setToolTip(tr("Reset the group of properties to its default state"));

    QHBoxLayout* heading = new QHBoxLayout();
    heading->setSpacing(PropertyWidgetQt::SPACING);
    heading->addWidget(btnCollapse_);
    heading->addWidget(label_);
    heading->addStretch(10);
    heading->addWidget(resetButton);

    QVBoxLayout* layout = new QVBoxLayout();
    setSpacingAndMargins(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addLayout(heading);
    layout->addWidget(propertyWidgetGroup_);

    setContentsMargins(1, 1, 1, 1);
    setObjectName("CollapsibleGroupBoxWidgetQt");
    setStyleSheet("#CollapsibleGroupBoxWidgetQt { border: 1px solid rgb(51, 51, 51); }");

    setLayout(layout);
}

QSize CollapsibleGroupBoxWidgetQt::sizeHint() const {
    QSize size = layout()->sizeHint();
    size.setWidth(std::max(PropertyWidgetQt::MINIMUM_WIDTH, size.width()));
    return size;
}

QSize CollapsibleGroupBoxWidgetQt::minimumSizeHint() const {
    QSize size = layout()->sizeHint();
    QSize minSize = layout()->minimumSize();
    size.setWidth(std::max(PropertyWidgetQt::MINIMUM_WIDTH, minSize.width()));
    return size;
}

void CollapsibleGroupBoxWidgetQt::updateFromProperty() {}

void CollapsibleGroupBoxWidgetQt::showWidget() {
    for (size_t i = 0; i < propertyWidgets_.size(); i++) {
        propertyWidgets_[i]->showWidget();
    }
    PropertyWidgetQt::showWidget();
}

void CollapsibleGroupBoxWidgetQt::hideWidget() {
    PropertyWidgetQt::hideWidget();
    for (size_t i = 0; i < propertyWidgets_.size(); i++) {
        propertyWidgets_[i]->hideWidget();
    }
}

void CollapsibleGroupBoxWidgetQt::toggleCollapsed() {
    if (collapsed_) {
        propertyWidgetGroup_->show();
        btnCollapse_->setIcon(QIcon(":/stylesheets/images/arrow_darker_down.png"));
    } else {
        propertyWidgetGroup_->hide();
        btnCollapse_->setIcon(QIcon(":/stylesheets/images/arrow_darker_right.png"));
    }
    collapsed_ = !collapsed_;
}

void CollapsibleGroupBoxWidgetQt::addProperty(Property* prop) {
    properties_.push_back(prop);

    PropertyWidgetQt* propertyWidget =
        static_cast<PropertyWidgetQt*>(PropertyWidgetFactory::getPtr()->create(prop));

    if (propertyWidget) {
        addWidget(propertyWidget);
        prop->registerWidget(propertyWidget);
        connect(propertyWidget, SIGNAL(usageModeChanged()), this, SLOT(updateContextMenu()));
        connect(propertyWidget, SIGNAL(updateSemantics(PropertyWidgetQt*)),
                this, SLOT(updatePropertyWidgetSemantics(PropertyWidgetQt*)));
        
        propertyWidget->hideWidget();
    } else {
        LogWarn("Could not find a widget for property: " << prop->getClassName());
    }
}

void CollapsibleGroupBoxWidgetQt::generateEventPropertyWidgets(EventPropertyManager* epm) {
    for (size_t i = 0; i < properties_.size(); i++) {
        const std::vector<PropertyWidget*>& widgets = properties_[i]->getWidgets();
        for (size_t j = 0; j < widgets.size(); j++) {
            EventPropertyWidgetQt* ep = dynamic_cast<EventPropertyWidgetQt*>(widgets[j]);
            if (ep) {
                ep->setManager(epm);
            }
        }
    }
}

std::string CollapsibleGroupBoxWidgetQt::getIdentifier() const { return identifier_; }

void CollapsibleGroupBoxWidgetQt::setIdentifier(const std::string& identifier) {
    identifier_ = identifier;
    label_->setText(identifier);
}

std::string CollapsibleGroupBoxWidgetQt::getDisplayName() const { return displayName_; }

void CollapsibleGroupBoxWidgetQt::setDisplayName(const std::string& displayName) {
    displayName_ = displayName;
}

std::vector<Property*> CollapsibleGroupBoxWidgetQt::getProperties() { return properties_; }

void CollapsibleGroupBoxWidgetQt::setGroupDisplayName() {
    displayName_ = label_->getText();
    label_->setText(displayName_);
    Property::setGroupDisplayName(identifier_, displayName_);
}

UsageMode CollapsibleGroupBoxWidgetQt::getUsageMode() const {
    UsageMode mode = DEVELOPMENT;
    for (size_t i = 0; i < propertyWidgets_.size(); i++) {
        mode = std::min(mode, propertyWidgets_[i]->getUsageMode());
    }
    return mode;
};

bool CollapsibleGroupBoxWidgetQt::getVisible() const {
    bool visible = false;
    for (size_t i = 0; i < propertyWidgets_.size(); i++) {
        visible = visible || propertyWidgets_[i]->getVisible();
    }
    return visible;
}

void CollapsibleGroupBoxWidgetQt::updateVisibility() {
    UsageMode appMode = getApplicationUsageMode();
    
    if (appMode >= getUsageMode()) {
        
        showWidget();
        
        for (size_t i = 0; i < properties_.size(); i++) {
            properties_[i]->updateVisibility();
        }
        
        for (size_t i = 0; i < propertyWidgets_.size(); i++) {
            CollapsibleGroupBoxWidgetQt* collapsiveWidget =
            dynamic_cast<CollapsibleGroupBoxWidgetQt*>(propertyWidgets_[i]);
            if (collapsiveWidget) {
                if (appMode >= collapsiveWidget->getUsageMode()) {
                    collapsiveWidget->showWidget();
                } else if (appMode < collapsiveWidget->getUsageMode() || !isVisible()) {
                    collapsiveWidget->hideWidget();
                }
                
                collapsiveWidget->updateVisibility();
            }
        }
        
    } else if (appMode < getUsageMode() || !isVisible()) {
        hideWidget();
    }
    
    updateContextMenu();
}

void CollapsibleGroupBoxWidgetQt::setDeveloperUsageMode(bool value) {
    for (size_t i = 0; i < propertyWidgets_.size(); i++) {
        propertyWidgets_[i]->setDeveloperUsageMode(value);
    }

    if(developerUsageModeAction_) {
        developerUsageModeAction_->setChecked(true);
    }
    updateWidgets();
    updateContextMenu();
}

void CollapsibleGroupBoxWidgetQt::setApplicationUsageMode(bool value) {
    for (size_t i = 0; i < propertyWidgets_.size(); i++) {
        propertyWidgets_[i]->setApplicationUsageMode(value);
    }

    if(applicationUsageModeAction_){
        applicationUsageModeAction_->setChecked(true);
    }
    updateWidgets();
    updateContextMenu();
}

void CollapsibleGroupBoxWidgetQt::updateWidgets() {
    for (size_t i = 0; i < propertyWidgets_.size(); i++)
        propertyWidgets_.at(i)->updateContextMenu();
}

void CollapsibleGroupBoxWidgetQt::serialize(IvwSerializer& s) const {
    s.serialize("collapsed", collapsed_);
    s.serialize("displayName", displayName_);
}

void CollapsibleGroupBoxWidgetQt::deserialize(IvwDeserializer& d) {
    bool value;
    std::string dispName;
    d.deserialize("collapsed", value);
    d.deserialize("displayName", dispName);
    displayName_ = dispName;
    collapsed_ = value;
}

void CollapsibleGroupBoxWidgetQt::addWidget(QWidget* widget) {
    propertyWidgetGroupLayout_->addWidget(widget);
    PropertyWidgetQt* propertyWidget = static_cast<PropertyWidgetQt*>(widget);
    if (std::find(propertyWidgets_.begin(), propertyWidgets_.end(), propertyWidget) ==
        propertyWidgets_.end()) {
        propertyWidgets_.push_back(propertyWidget);
    }
}

void CollapsibleGroupBoxWidgetQt::removeWidget(QWidget* widget) {
    propertyWidgetGroupLayout_->removeWidget(widget);
    PropertyWidgetQt* propertyWidget = static_cast<PropertyWidgetQt*>(widget);

    std::vector<PropertyWidgetQt*>::iterator it =
        std::find(propertyWidgets_.begin(), propertyWidgets_.end(), propertyWidget);
    
    if (it != propertyWidgets_.end()) {
        propertyWidgets_.erase(it);
    }
}

void CollapsibleGroupBoxWidgetQt::resetPropertyToDefaultState() {
    InviwoApplication::getPtr()->getProcessorNetwork()->lock();

    for (size_t i = 0; i < propertyWidgets_.size(); i++) {
        propertyWidgets_[i]->resetPropertyToDefaultState();
    }

    InviwoApplication::getPtr()->getProcessorNetwork()->unlock();
}

void CollapsibleGroupBoxWidgetQt::labelDidChange() {
    setDisplayName(label_->getText());
}

bool CollapsibleGroupBoxWidgetQt::isCollapsed() {
    return collapsed_;
}

    
void CollapsibleGroupBoxWidgetQt::updatePropertyWidgetSemantics(PropertyWidgetQt* widget) {
    Property* prop = widget->getProperty();
    
    bool visible = widget->isVisible();
    
    std::vector<Property*>::iterator pit =
        std::find(properties_.begin(), properties_.end(), prop);
    
    std::vector<PropertyWidgetQt*>::iterator wit =
        std::find(propertyWidgets_.begin(), propertyWidgets_.end(), widget);
    
    if (pit != properties_.end() && wit != propertyWidgets_.end()) {
        LogInfo("Found widget!");

        int layoutPosition = propertyWidgetGroupLayout_->indexOf(widget);
        
        PropertyWidgetQt* propertyWidget =
            static_cast<PropertyWidgetQt*>(PropertyWidgetFactory::getPtr()->create(prop));
        
        if (propertyWidget) {
            
            prop->deregisterWidget(widget);
            widget->hideWidget();
            removeWidget(widget);
            
            propertyWidgetGroupLayout_->insertWidget(layoutPosition, propertyWidget);
            propertyWidgets_.insert(wit, propertyWidget);
            prop->registerWidget(propertyWidget);
            
            connect(propertyWidget, SIGNAL(usageModeChanged()), this, SLOT(updateContextMenu()));
            connect(propertyWidget, SIGNAL(updateSemantics(PropertyWidgetQt*)),
                    this, SLOT(updatePropertyWidgetSemantics(PropertyWidgetQt*)));
            
            if (visible) {
                propertyWidget->showWidget();
            }else{
                propertyWidget->hideWidget();
            }
            
            
        } else {
            LogWarn("Could not change semantic for property: " << prop->getClassName());
        }
        
        
    }
}



}  // namespace
