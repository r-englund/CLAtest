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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/qt/editor/settingswidget.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/collapsiblegroupboxwidgetqt.h>
#include <inviwo/core/properties/propertywidgetfactory.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <QLayout>
#include <QFrame>
#include <QSettings>

namespace inviwo {

SettingsWidget::SettingsWidget(QString title, QWidget* parent) : InviwoDockWidget(title, parent) {
    generateWidget();
}

SettingsWidget::SettingsWidget(QWidget* parent) : InviwoDockWidget(tr("Settings"), parent) {
    generateWidget();
}

void SettingsWidget::generateWidget() {
    setObjectName("SettingsWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    tabWidget_ = new QTabWidget(this);
    setWidget(tabWidget_);
    //setWidget(frame);
}

SettingsWidget::~SettingsWidget() {
    while (propertyWidgets_.empty()) {
        delete propertyWidgets_.back();
        propertyWidgets_.pop_back();
    }
}

//Load settings from QSettings
void SettingsWidget::loadSettings() {
    std::vector<Settings*> settings = InviwoApplication::getPtr()->getModuleSettings();
    QSettings qmainsettings("Inviwo", "Inviwo");

    for (size_t i=0; i<settings.size(); i++) {
        //Holder widget
        QVBoxLayout* vLayout =  new QVBoxLayout();
        vLayout->setSpacing(0);
        vLayout->setAlignment(Qt::AlignTop);
        QWidget* tabHolder = new QWidget();
        tabHolder->setLayout(vLayout);
        //Scroll widget
        QScrollArea* scrollAreaTab = new QScrollArea(tabWidget_);
        scrollAreaTab->setWidgetResizable(true);
        scrollAreaTab->setMinimumWidth(300);
        scrollAreaTab->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollAreaTab->setFrameShape(QFrame::NoFrame);
        scrollAreaTab->setWidget(tabHolder);
        std::map<std::string, std::vector<Property*> > groups;
        std::vector<Property*> properties = settings[i]->getProperties();
        qmainsettings.beginGroup(tr(settings[i]->getIdentifier().c_str()));
        QStringList keys = qmainsettings.allKeys();

        for (size_t j=0; j<properties.size(); j++) {
            Property* curProperty = properties[j];
            QString name = QString::fromStdString(curProperty->getIdentifier());

            if (keys.contains(name)) {
                QVariant qval = qmainsettings.value(name);
                Variant val(std::string(qval.toString().toLocal8Bit().constData()));
                curProperty->setVariant(val);
            }

            if (curProperty->getGroupID() != "")
                groups[curProperty->getGroupID()].push_back(curProperty);
            else {
                PropertyWidgetQt* propertyWidget =
                    static_cast<PropertyWidgetQt*>(PropertyWidgetFactory::getRef().create(curProperty));
                curProperty->registerWidget(propertyWidget);
                propertyWidgets_.push_back(propertyWidget);
                vLayout->addWidget(propertyWidget);
            }
        }

        std::map<std::string, std::vector<Property*> >::iterator  groupIt;

        for (groupIt = groups.begin(); groupIt!=groups.end(); groupIt++) {
            Property* curProperty = groupIt->second[0];
            CollapsibleGroupBoxWidgetQt* group = new CollapsibleGroupBoxWidgetQt(curProperty->getGroupID(), curProperty->getGroupDisplayName());
            group->setIdentifier(curProperty->getGroupDisplayName());

            for (size_t j=0; j<groupIt->second.size(); j++) {
                Property* curProperty = groupIt->second[j];
                group->addProperty(curProperty);
            }

            group->generatePropertyWidgets();
            vLayout->addWidget(group);
        }

        qmainsettings.endGroup();
        tabWidget_->addTab(scrollAreaTab, tr(settings[i]->getIdentifier().c_str()));
        vLayout->addStretch(0);
    }
}

//Save application settings to QSettings
void SettingsWidget::saveSettings() {
    const std::vector<Settings*> settings = InviwoApplication::getRef().getModuleSettings();
    QSettings qmainsettings("Inviwo", "Inviwo");

    for (size_t i=0; i<settings.size(); i++) {
        std::vector<Property*> properties = settings[i]->getProperties();
        qmainsettings.beginGroup(tr(settings[i]->getIdentifier().c_str()));

        for (size_t j=0; j<properties.size(); j++) {
            Property* curProperty = properties[j];
            qmainsettings.setValue(QString::fromStdString(curProperty->getIdentifier()),
                                   QVariant(QString::fromStdString(curProperty->getVariant().getString())));
        }

        qmainsettings.endGroup();
    }
}

} // namespace