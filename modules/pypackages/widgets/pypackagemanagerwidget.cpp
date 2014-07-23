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
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

#include <modules/pypackages/widgets/pypackagemanagerwidget.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <apps/inviwo/inviwomainwindow.h>
#include <modules/pypackages/pyscriptrunner.h>
#include <QHeaderView>
#include <QSignalMapper>

#ifdef USING_QT5 
#define TABLE_WIDGET_SET_RESIZE_MODE setSectionResizeMode
#else
#define TABLE_WIDGET_SET_RESIZE_MODE setResizeMode
#endif

namespace inviwo {

PyPackageManagerWidget::PyPackageManagerWidget(QWidget* parent) : InviwoDockWidget(tr("Python Package Manager"), parent) {
    setObjectName("PyPackageManagerWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setFloating(true);
    init(this);
    QFrame* frame = new QFrame();
    vLayout_ = new QVBoxLayout(frame);
    
    packageTableWidget_ =  new QTableWidget();
    packageTableWidget_->horizontalHeader()->hide();
    packageTableWidget_->verticalHeader()->hide();
    packageTableWidget_->horizontalHeader()->TABLE_WIDGET_SET_RESIZE_MODE(QHeaderView::Stretch);
    packageTableWidget_->verticalHeader()->TABLE_WIDGET_SET_RESIZE_MODE(QHeaderView::Fixed);
    packageTableWidget_->setShowGrid(false);
    packageTableWidget_->setStyleSheet("QTableWidget::item { padding: 7px }");

    addPackages();

    //lineEdit_ = new QLineEdit(frame);
    //lineEdit_->setPlaceholderText("Package Name ...");
    //vLayout_->addWidget(lineEdit_);
    refreshButton_ = new QPushButton("Update Package Status");
    connect(refreshButton_, SIGNAL(clicked()), this, SLOT(refresh()));
    vLayout_->addWidget(packageTableWidget_);
    vLayout_->addWidget(refreshButton_);

    setWidget(frame);
    resize(500, 300);
    setVisible(false);

}

PyPackageManagerWidget::~PyPackageManagerWidget() {}

void PyPackageManagerWidget::updatePackages() {
    addPackages();
}

void PyPackageManagerWidget::addPackages() {
    
    clear();

    std::map<InviwoModule*, std::vector<std::string> > requirementMap = PyScriptRunner::getPtr()->getModulePackageRequirement();

    if (!requirementMap.size()) {
        return;
    }

    std::vector<std::string> packages;
    std::map<InviwoModule*, std::vector<std::string> >::iterator it;
    for (it=requirementMap.begin(); it!=requirementMap.end(); it++) {
        std::vector<std::string> modulePackages = it->second;
        for (size_t i=0; i<modulePackages.size(); i++) {
            if (std::find(packages.begin(), packages.end(), modulePackages[i])==packages.end()) {
                packages.push_back(modulePackages[i]);
            }
        }
    }

    packageTableWidget_->setRowCount((int)packages.size());
    packageTableWidget_->setColumnCount(3);

     QSignalMapper* signalMapper = new QSignalMapper(packageTableWidget_);

    for (size_t i=0; i<packages.size(); i++) {

        if (!PyScriptRunner::getPtr()->isPackageAvailable(packages[i])) {

            QTableWidgetItem* tableWidgetItem1 = new QTableWidgetItem();tableWidgetItem1->setFlags(Qt::ItemIsSelectable);
            QTableWidgetItem* tableWidgetItem2 = new QTableWidgetItem();tableWidgetItem2->setFlags(Qt::ItemIsSelectable);
            QTableWidgetItem* tableWidgetItem3 = new QTableWidgetItem();tableWidgetItem3->setFlags(Qt::ItemIsSelectable);

            packageTableWidget_->setItem(i, 0, tableWidgetItem1);
            packageTableWidget_->setItem(i, 1, tableWidgetItem2);
            packageTableWidget_->setItem(i, 2, tableWidgetItem3);

            QLabel* label = new QLabel(QString::fromStdString(packages[i]));
            label->setAlignment(Qt::AlignCenter);
            QPushButton* installButton = new QPushButton("Install");
            QPushButton* uinstallButton = new QPushButton("UnInstall");

            packageTableWidget_->setCellWidget(i, 0, label);
            packageTableWidget_->setCellWidget(i, 1, installButton);
            packageTableWidget_->setCellWidget(i, 2, uinstallButton);
            packageTableWidget_->resizeColumnsToContents();

            connect(installButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
            std::string installMapString = std::string("Install")+std::string("#")+packages[i];
            signalMapper->setMapping(installButton, QString::fromStdString(installMapString));

            connect(uinstallButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
            std::string uninstallMapString = std::string("UnInstall")+std::string("#")+packages[i];
            signalMapper->setMapping(uinstallButton, QString::fromStdString(uninstallMapString));
        }
        else {
            QTableWidgetItem* tableWidgetItem1 = new QTableWidgetItem();tableWidgetItem1->setFlags(Qt::ItemIsSelectable);
            QTableWidgetItem* tableWidgetItem2 = new QTableWidgetItem();tableWidgetItem2->setFlags(Qt::ItemIsSelectable);
            QTableWidgetItem* tableWidgetItem3 = new QTableWidgetItem();tableWidgetItem3->setFlags(Qt::ItemIsSelectable);

            packageTableWidget_->setItem(i, 0, tableWidgetItem1);
            packageTableWidget_->setItem(i, 1, tableWidgetItem2);
            packageTableWidget_->setItem(i, 2, tableWidgetItem3);
            
            QLabel* label = new QLabel(QString::fromStdString(packages[i]));
            label->setAlignment(Qt::AlignCenter);
            QPushButton* upgradeButton = new QPushButton("Upgrade");
            QPushButton* uinstallButton = new QPushButton("UnInstall");

            packageTableWidget_->setCellWidget(i, 0, label);
            packageTableWidget_->setCellWidget(i, 1, upgradeButton);
            packageTableWidget_->setCellWidget(i, 2, uinstallButton);
            packageTableWidget_->resizeColumnsToContents();

            connect(upgradeButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
            std::string upgradeMapString = std::string("Upgrade")+std::string("#")+packages[i];
            signalMapper->setMapping(upgradeButton, QString::fromStdString(upgradeMapString));

            connect(uinstallButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
            std::string uninstallMapString = std::string("UnInstall")+std::string("#")+packages[i];
            signalMapper->setMapping(uinstallButton, QString::fromStdString(uninstallMapString));
        }

        //packageTableWidget_->resizeRowsToContents();
    }

    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(mappedButtonClick(QString)));
}

void PyPackageManagerWidget::clear() {
    packageTableWidget_->clear();
}

void PyPackageManagerWidget::show() {
    setVisible(true);
}

void PyPackageManagerWidget::mappedButtonClick(QString str) {
    std::string encodedPackageName = std::string(str.toLocal8Bit().constData());
    std::vector<std::string> decodedString = splitString(encodedPackageName, '#');
    if (decodedString.size()!=2) {
        LogWarn("Unable to process signals");
    }

    std::string packageName = decodedString[1];

    if (decodedString[0] == "Upgrade") {
        LogWarn(" Requested upgrade of " << packageName)
    }
    else if (decodedString[0] == "UnInstall") {
        LogWarn(" Requested uninstall of " << packageName)
        PyScriptRunner::getPtr()->uninstallPackage(packageName);
    }
    else if (decodedString[0] == "Install") {
        LogWarn(" Requested install of " << packageName)
        PyScriptRunner::getPtr()->installPackage(packageName);
    }
}

void PyPackageManagerWidget::refresh() {
    addPackages();
}

} // namespace