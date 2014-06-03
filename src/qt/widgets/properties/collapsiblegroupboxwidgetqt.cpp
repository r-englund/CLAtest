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
#include <inviwo/qt/widgets/properties/camerapropertywidgetqt.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/settings/systemsettings.h>
#include <inviwo/core/properties/propertywidgetfactory.h>
#include <inviwo/core/network/processornetwork.h>

namespace inviwo {

CollapsibleGroupBoxWidgetQt::CollapsibleGroupBoxWidgetQt(std::string identifier, std::string displayName)
    : PropertyWidgetQt()
    , identifier_(identifier)
    , displayName_(displayName)
    , collapsed_(false)
    , visibilityMode_(APPLICATION)
    , contextMenu_(NULL)
    , viewModeActionGroup_(NULL) 
    , viewModeItem_(NULL) {

    setObjectName("CollapsibleGroupBoxWidgetQt");
    generateWidget();
    updateFromProperty();
}

void CollapsibleGroupBoxWidgetQt::generateWidget() {
    vLayout_ = new QVBoxLayout();
    vLayout_->setAlignment(Qt::AlignRight);
    vLayout_->setAlignment(Qt::AlignTop);
    vLayout_->setContentsMargins(7,7,7,7);
    vLayout_->setSpacing(7);

    groupBox_ = new QGroupBox(this);
    groupBox_->setFlat(true);
    groupBox_->setLayout(vLayout_);

    QHBoxLayout* heading = new QHBoxLayout();
    heading->setSpacing(7);

    btnCollapse_ = new QToolButton(this);
    btnCollapse_->setIcon(QIcon(":/stylesheets/images/arrow_darker_down.png"));
    connect(btnCollapse_, 
            SIGNAL(clicked()), 
            this, 
            SLOT(toggleFold()));

    heading->addWidget(btnCollapse_);
    label_ = new EditableLabelQt(this, displayName_, false);
    heading->addWidget(label_);
    heading->addStretch(10);
    QToolButton* resetButton = new QToolButton(this);
    resetButton->setIcon(QIcon(":/icons/edit-undo-icon.png"));
    resetButton->setIconSize(QSize(20, 20));

    connect(resetButton,
            SIGNAL(clicked()),
            this,
            SLOT(resetPropertyToDefaultState()));
    resetButton->setToolTip(tr("&Reset the group of properties to its default state"));
 
    heading->addWidget(resetButton);

    
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setContentsMargins(0,0,0,0);
    gridLayout->setSpacing(0);
    gridLayout->addLayout(heading, 1, 0, Qt::AlignLeft);
    gridLayout->addWidget(groupBox_, 2, 0);
    
    QFrame* frame = new QFrame(this);
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    frame->setLineWidth(2);
    frame->setLayout(gridLayout);
 
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(frame);
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(0);
 
    setLayout(hLayout);
    
}

void CollapsibleGroupBoxWidgetQt::updateFromProperty() {}

void CollapsibleGroupBoxWidgetQt::toggleFold() {
    if(collapsed_) {
        groupBox_->show();
        btnCollapse_->setIcon(QIcon(":/stylesheets/images/arrow_darker_down.png"));
    } else {
        groupBox_->hide();
        btnCollapse_->setIcon(QIcon(":/stylesheets/images/arrow_darker_right.png"));
    }
    collapsed_ = !collapsed_;
    
}

void CollapsibleGroupBoxWidgetQt::addProperty(Property* prop) {
    properties_.push_back(prop);
    
    PropertyWidgetQt* propertyWidget =
            static_cast<PropertyWidgetQt*>(PropertyWidgetFactory::getPtr()->create(prop));

    if (propertyWidget) {
        if(prop->getVisibilityMode() == INVISIBLE)
            propertyWidget->setVisible(false);
        addWidget(propertyWidget);
        prop->registerWidget(propertyWidget);
        connect(propertyWidget, SIGNAL(modified()), this, SLOT(propertyModified()));
        //connect(propertyWidget, SIGNAL(visibilityChange()), this, SLOT(updateVisibility()));
    } else{
        LogWarn("Could not find a widget for property: " << prop->getClassName());
    }
}

void CollapsibleGroupBoxWidgetQt::generateEventPropertyWidgets(EventPropertyManager* epm) {
    for (size_t i=0; i<properties_.size(); i++) {
        const std::vector<PropertyWidget*>& widgets = properties_[i]->getWidgets();
        for (size_t j=0; j<widgets.size(); j++) {
            EventPropertyWidgetQt* ep = dynamic_cast<EventPropertyWidgetQt*>(widgets[j]);
            if (ep) {
                ep->setManager(epm);
            }
        }
    }
}

std::string CollapsibleGroupBoxWidgetQt::getIdentifier() const {
    return identifier_;
}

void CollapsibleGroupBoxWidgetQt::setIdentifier(const std::string& identifier) {
    identifier_ = identifier;
    label_->setText(identifier);
}

std::string CollapsibleGroupBoxWidgetQt::getDisplayName() const {
    return displayName_;
}

void CollapsibleGroupBoxWidgetQt::setDisplayName(const std::string& displayName) {
    displayName_ = displayName;
}

std::vector<Property*> CollapsibleGroupBoxWidgetQt::getProperties() {
    return properties_;
}

void CollapsibleGroupBoxWidgetQt::propertyModified() {
    emit modified();
}

void CollapsibleGroupBoxWidgetQt::setGroupDisplayName() {
    displayName_ = label_->getText();
    label_->setText(displayName_);
    Property::setGroupDisplayName(identifier_, displayName_);
}

void CollapsibleGroupBoxWidgetQt::updateVisibility() {
    for (size_t i=0; i<properties_.size(); i++) {
        properties_[i]->updateVisibility();
    }

    PropertyVisibilityMode appMode  = getApplicationViewMode();
    for (size_t i = 0; i < propertyWidgets_.size(); i++) {
        CollapsibleGroupBoxWidgetQt* collapsiveWidget = dynamic_cast<CollapsibleGroupBoxWidgetQt*>(propertyWidgets_[i]);
        if(collapsiveWidget){
            if(appMode > collapsiveWidget->getVisibilityMode())
                collapsiveWidget->setVisible(false);
            else if(isVisible())
                collapsiveWidget->setVisible(true);
            collapsiveWidget->updateVisibility();
        }
    }
}

void CollapsibleGroupBoxWidgetQt::setDeveloperViewMode(bool value) {
    visibilityMode_ = DEVELOPMENT;

    for (size_t i=0; i<properties_.size(); i++)
        properties_.at(i)->setVisibility(DEVELOPMENT);

    developerViewModeAction_->setChecked(true);
    updateWidgets();
    PropertyVisibilityMode visibilityMode  = getApplicationViewMode();

    if (visibilityMode == DEVELOPMENT)
        this->setVisible(true);

    if (visibilityMode == APPLICATION)
        this->setVisible(false);

    emit visibilityModified();
}

void CollapsibleGroupBoxWidgetQt::setApplicationViewMode(bool value) {
    visibilityMode_ = APPLICATION;

    for (size_t i=0; i<properties_.size(); i++)
        properties_.at(i)->setVisibility(APPLICATION);

    applicationViewModeAction_->setChecked(true);
    updateWidgets();
    this->setVisible(true);
    emit visibilityModified();
}


void CollapsibleGroupBoxWidgetQt::updateWidgets() {
    for (size_t i=0; i<propertyWidgets_.size(); i++)
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
    vLayout_->addWidget(widget);
    PropertyWidgetQt* propertyWidget = static_cast<PropertyWidgetQt*>(widget);

    if (propertyWidget) {
        if (std::find(propertyWidgets_.begin(), propertyWidgets_.end(), propertyWidget) == propertyWidgets_.end())
            propertyWidgets_.push_back(propertyWidget);
    }
}

void CollapsibleGroupBoxWidgetQt::removeWidget(QWidget* widget) {
    vLayout_->removeWidget(widget);
    PropertyWidgetQt* propertyWidget = static_cast<PropertyWidgetQt*>(widget);

    if (propertyWidget) {
        if (std::find(propertyWidgets_.begin(), propertyWidgets_.end(), propertyWidget) != propertyWidgets_.end()) {
            std::vector<PropertyWidgetQt*>::iterator it = std::find(propertyWidgets_.begin(), propertyWidgets_.end(), propertyWidget);
            propertyWidgets_.erase(it);
        }
    }
}

void CollapsibleGroupBoxWidgetQt::onViewModeChange() {
    // TODO figure out what to do here.... // Peter 20140225
}

void CollapsibleGroupBoxWidgetQt::resetPropertyToDefaultState() {
    InviwoApplication::getPtr()->getProcessorNetwork()->lock();

    for (size_t i = 0; i < properties_.size(); i++) {
        properties_[i]->resetToDefaultState();
    }
    
    for (size_t i = 0; i < propertyWidgets_.size(); i++) {
        CollapsibleGroupBoxWidgetQt* widget = dynamic_cast<CollapsibleGroupBoxWidgetQt*>(propertyWidgets_[i]);
        if (widget) {
            widget->resetPropertyToDefaultState();
        }

        CompositePropertyWidgetQt* compWidget = dynamic_cast<CompositePropertyWidgetQt*>(propertyWidgets_[i]);
        if (compWidget) {
            compWidget->resetPropertyToDefaultState();
        }

        CameraPropertyWidgetQt* camWidget = dynamic_cast<CameraPropertyWidgetQt*>(propertyWidgets_[i]);
        if(camWidget) {
            camWidget->resetPropertyToDefaultState();
        }
    }

    InviwoApplication::getPtr()->getProcessorNetwork()->unlock();
}

} // namespace
