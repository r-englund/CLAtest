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
 * Main file authors: Sathish Kottravel, Erik Sundén
 *
 *********************************************************************************/

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