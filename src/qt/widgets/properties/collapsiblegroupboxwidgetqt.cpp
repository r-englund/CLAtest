/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
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

#include <inviwo/qt/widgets/properties/collapsiblegroupboxwidgetqt.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/settings/systemsettings.h>
#include <inviwo/core/properties/propertywidgetfactory.h>

namespace inviwo {

CollapsibleGroupBoxWidgetQt::CollapsibleGroupBoxWidgetQt(std::string identifier, std::string displayName)
    : identifier_(identifier)
    , displayName_(displayName)
    , visibilityMode_(APPLICATION) {
    collapsed_ = false;
    generateWidget();
    updateFromProperty();
}

void CollapsibleGroupBoxWidgetQt::generateWidget() {
    setContextMenuPolicy(Qt::CustomContextMenu);
    btnCollapse_ = new QToolButton(this);
    btnCollapse_->setStyleSheet("QToolButton::hover { background-color: gray }");
    QHBoxLayout* hLayout = new QHBoxLayout();
    QGridLayout* gridLayout = new QGridLayout();
    QHBoxLayout* H2 = new QHBoxLayout();
    groupBox_ = new QGroupBox(this);
    groupBox_->setFlat(true);
    groupBox_->setStyleSheet("border:0;");
    QFrame* frame = new QFrame(this);
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    frame->setLineWidth(2);
    hLayout->addWidget(frame);
    vLayout_ = new QVBoxLayout();
    vLayout_->setAlignment(Qt::AlignRight);
    vLayout_->setAlignment(Qt::AlignTop);
    vLayout_->setMargin(0);
    vLayout_->setSpacing(0);
    groupBox_->setLayout(vLayout_);
    gridLayout->setContentsMargins(0,0,0,0);
    gridLayout->setSpacing(0);
    H2->addWidget(btnCollapse_);
    label_ = new EditableLabelQt(this,displayName_);
    H2->addWidget(label_);
    H2->setSpacing(7);
    gridLayout->addLayout(H2,1,0,Qt::AlignLeft);
    gridLayout->addWidget(groupBox_,2,0);
    frame->setLayout(gridLayout);
    setLayout(hLayout);
    show();
}

void CollapsibleGroupBoxWidgetQt::updateFromProperty() {}

void CollapsibleGroupBoxWidgetQt::show() {
    collapsed_ = false;
    groupBox_->show();
    btnCollapse_->setIcon(QIcon(":/stylesheets/images/arrow_darker_down.png"));
    disconnect(btnCollapse_, SIGNAL(clicked()), this, SLOT(show()));
    connect(btnCollapse_, SIGNAL(clicked()), this, SLOT(hide()));
}

void CollapsibleGroupBoxWidgetQt::hide() {
    collapsed_= true;
    groupBox_->hide();
    btnCollapse_->setIcon(QIcon(":/stylesheets/images/arrow_darker_right.png"));
    disconnect(btnCollapse_, SIGNAL(clicked()), this, SLOT(hide()));
    connect(btnCollapse_, SIGNAL(clicked()), this, SLOT(show()));
}

void CollapsibleGroupBoxWidgetQt::addProperty(Property* tmpProperty) {
    properties_.push_back(tmpProperty);
}

void CollapsibleGroupBoxWidgetQt::generatePropertyWidgets() {
    for (size_t i=0; i<properties_.size(); i++) {
        Property* curProperty = properties_[i];
        PropertyWidgetQt* propertyWidget =
            static_cast<PropertyWidgetQt*>(PropertyWidgetFactory::getPtr()->create(curProperty));

        if (propertyWidget) {
            vLayout_->addWidget(propertyWidget);
            curProperty->registerWidget(propertyWidget);
            propertyWidgets_.push_back(propertyWidget);
            connect(propertyWidget, SIGNAL(modified()), this, SLOT(propertyModified()));
            connect(propertyWidget, SIGNAL(visibilityChange()), this, SLOT(updateVisibility()));
        } else
            LogWarn("Could not find a widget for property: " << curProperty->getClassName());
    }

    generateContextMenu();
}

void CollapsibleGroupBoxWidgetQt::generateEventPropertyWidgets(EventPropertyManager* eventPropertyManager) {
    for (size_t i=0; i<properties_.size(); i++) {
        Property* curProperty = properties_[i];
        PropertyWidgetQt* propertyWidget =
            static_cast<PropertyWidgetQt*>(PropertyWidgetFactory::getPtr()->create(curProperty));
        vLayout_->addWidget(propertyWidget);
        curProperty->registerWidget(propertyWidget);
        dynamic_cast<EventPropertyWidgetQt*>(propertyWidget)->setManager(eventPropertyManager);
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
        this->setContextMenuPolicy(Qt::CustomContextMenu);

        for (size_t i= 0; i<properties_.size(); i++) {
            if (properties_[i]->getVisibilityMode()!= INVISIBLE) {
                this->setVisible(true);
                break;
            }

            this->setVisible(false);
        }
    }

    if (visibilityMode == APPLICATION) {
        this->setContextMenuPolicy(Qt::NoContextMenu);

        for (size_t i= 0; i<properties_.size(); i++) {
            if (properties_[i]->getVisibilityMode()== APPLICATION) {
                this->setVisible(true);
                break;
            }

            this->setVisible(false);
        }
    }
}

void CollapsibleGroupBoxWidgetQt::generateContextMenu() {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    this->addObservation(static_cast<OptionPropertyInt*>(inviwoApp->getSettingsByType<SystemSettings>()->getPropertyByIdentifier("viewMode")));
    this->setContextMenuPolicy(Qt::CustomContextMenu);
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
    updateContextMenu();
    connect(this,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint&)));
    connect(developerViewModeAction_,SIGNAL(triggered(bool)),this, SLOT(setDeveloperViewMode(bool)));
    connect(applicationViewModeAction_,SIGNAL(triggered(bool)),this, SLOT(setApplicationViewMode(bool)));
}

void CollapsibleGroupBoxWidgetQt::showContextMenu(const QPoint& pos) {
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
    s.serialize("displayName",displayName_);
}

void CollapsibleGroupBoxWidgetQt::deserialize(IvwDeserializer& d) {
    bool value;
    std::string dispName;
    d.deserialize("collapsed", value);
    d.deserialize("displayName",dispName);
    displayName_ = dispName;
    collapsed_ = value;
}

void CollapsibleGroupBoxWidgetQt::addWidget(QWidget* widget) {
    vLayout_->addWidget(widget);
}

} // namespace
