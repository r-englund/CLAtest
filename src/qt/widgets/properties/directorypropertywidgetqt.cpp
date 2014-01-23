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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/qt/widgets/properties/directorypropertywidgetqt.h>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif

#include <QDir>
#include <QFileDialog>
#include <QList>
#include <QSettings>
#include <QUrl>

namespace inviwo {

DirectoryPropertyWidgetQt::DirectoryPropertyWidgetQt(DirectoryProperty* property) : property_(property) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();
}

void DirectoryPropertyWidgetQt::generateWidget() {
    setObjectName("DirectoryPropertyWidgetQt");
    QHBoxLayout* hLayout = new QHBoxLayout();
    directoryLabel_ = new EditableLabelQt(property_->getDisplayName(),PropertyWidgetQt::generatePropertyWidgetMenu());
    hLayout->addWidget(directoryLabel_);
    lineEdit_ = new QLineEdit();
    lineEdit_->setReadOnly(true);
    openButton_ = new QToolButton();
    openButton_->setIcon(QIcon(":/icons/open.png"));

    connect(directoryLabel_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    connect(openButton_, SIGNAL(pressed()), this, SLOT(setPropertyValue()));
    hLayout->addWidget(lineEdit_);
    hLayout->addWidget(openButton_);
    setLayout(hLayout);
}

void DirectoryPropertyWidgetQt::setPropertyValue() {
    QString dataDir;

    if (property_->get().empty())
        dataDir = QString::fromStdString(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_DATA));
    else
        dataDir = QString::fromStdString(property_->get());

    QList<QUrl> sidebarURLs;
    sidebarURLs << QUrl::fromLocalFile(QDir(dataDir).absolutePath());

    //TODO: create InviwoFileDialog to avoid frequent version checks
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    sidebarURLs << QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    sidebarURLs << QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
#else
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
#endif

    QFileDialog openFileDialog(this, tr("Open Directory ..."), QDir(dataDir).absolutePath());
    openFileDialog.setFileMode(QFileDialog::Directory);
    openFileDialog.setSidebarUrls(sidebarURLs);

    QString existingDir = openFileDialog.getExistingDirectory();
    std::string dir = existingDir.toLocal8Bit().constData();

    if (!dir.empty()) {
        setPropertyTreeInfo(dir);
        property_->set(dir);
        emit modified();
    }
}

void DirectoryPropertyWidgetQt::updateFromProperty() {
    QDir currentDir = QDir(QString::fromStdString(property_->get()));
    lineEdit_->setText(currentDir.dirName());
    setPropertyTreeInfo(property_->get());
}

void DirectoryPropertyWidgetQt::setPropertyTreeInfo(std::string path) {
    QDir currentDir = QDir(QString::fromStdString(path));
    lineEdit_->setText(currentDir.dirName());
    QStringList files;
    QString filter = "*.*";
    files = currentDir.entryList(QStringList(filter), QDir::Files | QDir::NoSymLinks);

    std::vector<std::string> directoryTreeInfo;
    //TODO: set property tree info
    for (int i=0; i<files.size(); i++) {
        std::string fileName = files[i].toLocal8Bit().constData();
        directoryTreeInfo.push_back(fileName);
    }

    property_->setDirectoryTree(directoryTreeInfo);
}

void DirectoryPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(directoryLabel_->getText());
}


} // namespace
