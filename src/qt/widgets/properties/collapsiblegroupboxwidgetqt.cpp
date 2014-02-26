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

namespace inviwo {

CollapsibleGroupBoxWidgetQt::CollapsibleGroupBoxWidgetQt(std::string identifier, std::string displayName)
    : PropertyWidgetQt()
    , identifier_(identifier)
    , displayName_(displayName)
    , visibilityMode_(APPLICATION)
    , collapsed_(false)
    , contextMenu_(NULL)
    , viewModeActionGroup_(NULL) 
    , viewModeItem_(NULL) {

    setObjectName("CollapsibleGroupBoxWidgetQt");
  
    OptionPropertyInt* viewmode = static_cast<OptionPropertyInt*>(
        InviwoApplication::getPtr()->
            getSettingsByType<SystemSettings>()->
                getPropertyByIdentifier("viewMode")
        );

    viewmode->onChange(this, &CollapsibleGroupBoxWidgetQt::onViewModeChange);

    generateWidget();
    updateFromProperty();
}

void CollapsibleGroupBoxWidgetQt::generateWidget() {
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(showContextMenu(const QPoint&)));

    vLayout_ = new QVBoxLayout();
    vLayout_->setAlignment(Qt::AlignRight);
    vLayout_->setAlignment(Qt::AlignTop);
    vLayout_->setContentsMargins(0,0,0,0);
    vLayout_->setSpacing(11);

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
    label_ = new EditableLabelQt(this, displayName_, PropertyWidgetQt::getContextMenu());
    heading->addWidget(label_);
    heading->addStretch(10);
    QPushButton* resetButton = new QPushButton(tr("&Reset"), this);
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
        addWidget(propertyWidget);
        prop->registerWidget(propertyWidget);
        connect(propertyWidget, SIGNAL(modified()), this, SLOT(propertyModified()));
        connect(propertyWidget, SIGNAL(visibilityChange()), this, SLOT(updateVisibility()));
    } else{
        LogWarn("Could not find a widget for property: " << prop->getClassName());
    }
}

void CollapsibleGroupBoxWidgetQt::generateEventPropertyWidgets(EventPropertyManager* epm) {
    for (size_t i=0; i<properties_.size(); i++) {
        EventPropertyWidgetQt* ep = dynamic_cast<EventPropertyWidgetQt*>(properties_[i]);
        if (ep) {
            ep->setManager(epm);
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
    PropertyVisibilityMode visibilityMode  = getApplicationViewMode();

    if (visibilityMode == DEVELOPMENT) {
        for (size_t i=0; i<properties_.size(); i++) {
            if (properties_[i]->getVisibilityMode() != INVISIBLE) {
                this->setVisible(true);
                break;
            }
            this->setVisible(false);
        }
    }else if (visibilityMode == APPLICATION) {
         for (size_t i=0; i<properties_.size(); i++) {
            if (properties_[i]->getVisibilityMode()== APPLICATION) {
                this->setVisible(true);
                break;
            }
            this->setVisible(false);
        }
    }
}

void CollapsibleGroupBoxWidgetQt::generateContextMenu() {
    contextMenu_ = new QMenu(this);
    QMenu* viewModeItem_ = new QMenu(tr("&View mode"),contextMenu_);
    developerViewModeAction_ = new QAction(tr("&Developer"),this);
    developerViewModeAction_->setCheckable(true);
    viewModeItem_->addAction(developerViewModeAction_);
    applicationViewModeAction_ = new QAction(tr("&Application"),this);
    applicationViewModeAction_->setCheckable(true);
    viewModeItem_->addAction(applicationViewModeAction_);
    viewModeActionGroup_ = new QActionGroup(this);
    viewModeActionGroup_->addAction(developerViewModeAction_);
    viewModeActionGroup_->addAction(applicationViewModeAction_);
    contextMenu_->addMenu(viewModeItem_);
    resetAction_ = new QAction(tr("&Reset"), this);
    contextMenu_->addAction(resetAction_);
    
    updateContextMenu();
    
    connect(developerViewModeAction_,
            SIGNAL(triggered(bool)),
            this,
            SLOT(setDeveloperViewMode(bool)));
    
    connect(applicationViewModeAction_,
            SIGNAL(triggered(bool)),
            this,
            SLOT(setApplicationViewMode(bool)));

    connect(resetAction_,
            SIGNAL(triggered(bool)),
            this,
            SLOT(resetPropertyToDefaultState()));
}

void CollapsibleGroupBoxWidgetQt::showContextMenu(const QPoint& pos) {
    
    if(!contextMenu_){
        generateContextMenu();
    }

    PropertyVisibilityMode appVisibilityMode  = getApplicationViewMode();

    if (appVisibilityMode == DEVELOPMENT) {
        updateContextMenu();
        QPoint globalPos = this->mapToGlobal(pos);
        contextMenu_->exec(globalPos);
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

void CollapsibleGroupBoxWidgetQt::updateContextMenu() {
    if (visibilityMode_ == DEVELOPMENT)
        developerViewModeAction_->setChecked(true);

    if (visibilityMode_ == APPLICATION)
        applicationViewModeAction_->setChecked(true);
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
    PropertyOwner::InvalidationLevel invalidationLevel = PropertyOwner::VALID;
    for (size_t i = 0; i < properties_.size(); i++) {
        properties_[i]->lockInvalidation(true);
        properties_[i]->resetToDefaultState();
        properties_[i]->lockInvalidation(false);
        invalidationLevel = std::max(invalidationLevel, properties_[i]->getInvalidationLevel());
    }
    if (properties_[0]->getOwner()) {
        properties_[0]->getOwner()->invalidate(invalidationLevel, properties_[0]);
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
}

} // namespace
