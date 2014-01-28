/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/qt/widgets/properties/filepropertywidgetqt.h>

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

FilePropertyWidgetQt::FilePropertyWidgetQt(FileProperty* property) : property_(property) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();
}

void FilePropertyWidgetQt::generateWidget() {
    setObjectName("FilePropertyWidgetQt");
    QHBoxLayout* hLayout = new QHBoxLayout();
    label_ = new EditableLabelQt(this,property_->getDisplayName(),PropertyWidgetQt::generatePropertyWidgetMenu());
    hLayout->addWidget(label_);
    lineEdit_ = new QLineEdit(this);
    lineEdit_->setReadOnly(true);
    openButton_ = new QToolButton(this);
    openButton_->setIcon(QIcon(":/icons/open.png"));

    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    connect(openButton_, SIGNAL(pressed()), this, SLOT(setPropertyValue()));
    hLayout->addWidget(lineEdit_);
    hLayout->addWidget(openButton_);
    setLayout(hLayout);
}

void FilePropertyWidgetQt::setPropertyValue() {
    // dialog window settings

    // Setup sidebar
    QList<QUrl> sidebarURLs;
    QString dataDir_ = QString::fromStdString(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_DATA));
    sidebarURLs << QUrl::fromLocalFile(QDir(dataDir_).absolutePath());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    sidebarURLs << QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    sidebarURLs << QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
#else
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::DesktopLocation));
    sidebarURLs << QUrl::fromLocalFile(QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
#endif

    // Setup default path
    QString path;
    if (property_->get()!= "") {
        path=QDir(QString::fromStdString(property_->get())).absolutePath();
    } else {
        path=QDir(dataDir_).absolutePath();
    }

    // Setup Extensions
    std::vector<std::string> filters = property_->getNameFilters();
    QStringList extension;
    for (std::vector<std::string>::const_iterator it = filters.begin();
         it!=filters.end(); ++it) {
            extension.push_back(QString::fromStdString(*it));
    }

    QFileDialog importFileDialog(this, QString::fromStdString(property_->getDisplayName()), path);

    switch (property_->getAcceptMode()){
    case FileProperty::AcceptSave :
        importFileDialog.setAcceptMode(QFileDialog::AcceptSave);
        break;
    case FileProperty::AcceptOpen :
        importFileDialog.setAcceptMode(QFileDialog::AcceptOpen);
        break;
    default:
        importFileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    }

    switch (property_->getFileMode()){
    case FileProperty::AnyFile:
        importFileDialog.setFileMode(QFileDialog::AnyFile);
        break;
    case FileProperty::ExistingFile:
        importFileDialog.setFileMode(QFileDialog::ExistingFile);
        break;
    case FileProperty::Directory:
        importFileDialog.setFileMode(QFileDialog::Directory);
        break;
    case FileProperty::ExistingFiles:
        importFileDialog.setFileMode(QFileDialog::ExistingFiles);
        break;
    case FileProperty::DirectoryOnly:
        importFileDialog.setFileMode(QFileDialog::DirectoryOnly);
        break;
    default:
        importFileDialog.setFileMode(QFileDialog::AnyFile);
    }

    importFileDialog.setNameFilters(extension);
    importFileDialog.setSidebarUrls(sidebarURLs);

    if (importFileDialog.exec()) {
        QString path = importFileDialog.selectedFiles().at(0);
        property_->set(path.toLocal8Bit().constData());
    }
    updateFromProperty();
    emit modified();
}

void FilePropertyWidgetQt::updateFromProperty() {
    lineEdit_->setText(QFileInfo(QString::fromStdString(property_->get())).fileName());
}

void FilePropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}


} // namespace
