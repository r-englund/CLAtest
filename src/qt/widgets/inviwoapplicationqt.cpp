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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
#include <inviwo/core/util/settings/systemsettings.h>
#include <inviwo/qt/widgets/qtwidgetmodule.h>
#include <QFile>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QSound>
#endif

#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#else
#include <time.h>
#endif

namespace inviwo {

InviwoApplicationQt::InviwoApplicationQt(std::string displayName, 
                                         std::string basePath,
                                         int& argc,
                                         char** argv)
    : QApplication(argc, argv)
    , InviwoApplication(argc, argv, displayName, basePath) {

    QCoreApplication::setOrganizationName("Inviwo");
    QCoreApplication::setOrganizationDomain("inviwo.org");
    QCoreApplication::setApplicationName(displayName.c_str());

    // initialize singletons
    //ProcessorWidgetFactory::init();
    PropertyWidgetFactoryQt::init();

    fileWatcher_ = new QFileSystemWatcher(this);
    connect(fileWatcher_, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));

    // Since QtWidgets are not a module we have to register it our self
    registerModule(new QtWidgetModule());
}

InviwoApplicationQt::~InviwoApplicationQt(){}

void InviwoApplicationQt::setMainWindow(QMainWindow* mainWindow) { 
    mainWindow_ = mainWindow; 
}

void InviwoApplicationQt::registerFileObserver(FileObserver* fileObserver) {
    ivwAssert(std::find(fileObservers_.begin(),fileObservers_.end(),fileObserver)==fileObservers_.end(),
              "File observer already registered.");
    fileObservers_.push_back(fileObserver);
}

void InviwoApplicationQt::startFileObservation(std::string fileName) {
    QString qFileName = QString::fromStdString(fileName);
    if (!fileWatcher_->files().contains(qFileName)){
        fileWatcher_->addPath(qFileName); 
    }
}

void InviwoApplicationQt::stopFileObservation(std::string fileName) {
    QString qFileName = QString::fromStdString(fileName);
    ivwAssert(fileWatcher_->files().contains(qFileName),
              "trying to stop observation of an unobserved file: "+fileName);
    if (fileWatcher_->files().contains(qFileName)){
        fileWatcher_->removePath(qFileName);
    }
}

void InviwoApplicationQt::fileChanged(QString fileName) {
    wait(200);
    if(QFile::exists(fileName)){
        std::string fileNameStd = fileName.toLocal8Bit().constData();
        for (size_t i=0; i<fileObservers_.size(); i++) {
            std::vector<std::string> observedFiles = fileObservers_[i]->getFiles();
            if (std::find(observedFiles.begin(), observedFiles.end(), fileNameStd) != observedFiles.end())
                fileObservers_[i]->fileChanged(fileNameStd);
        }
    }
}

void InviwoApplicationQt::closeInviwoApplication(){
	this->quit();
}

void InviwoApplicationQt::playSound(unsigned int soundID) {
    // Qt currently does not support playing sounds from resources
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    if((dynamic_cast<BoolProperty*>(InviwoApplication::getPtr()->getSettingsByType<SystemSettings>()->getPropertyByIdentifier("enableSound"))->get())){
        if (soundID == IVW_OK) QSound::play(QString::fromStdString(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_RESOURCES)+"sounds/ok.wav"));
        else if (soundID == IVW_ERROR) QSound::play(QString::fromStdString(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_RESOURCES)+"sounds/error.wav"));
    }
#endif
}


void InviwoApplicationQt::initialize(registerModuleFuncPtr regModuleFunc){
	LogInfoCustom("InviwoInfo","QT Version " << QT_VERSION_STR);
	InviwoApplication::initialize(regModuleFunc);
}

Timer* InviwoApplicationQt::createTimer()const { 
    return new TimerQt(); 
}

void InviwoApplicationQt::wait(int ms){
    if(ms <= 0)
        return;

#ifdef Q_OS_WIN
        Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

} // namespace
