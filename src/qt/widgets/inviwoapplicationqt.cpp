 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file authors: Erik Sundén, Timo Ropinski
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/inviwoapplicationqt.h>
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
    QCoreApplication::setOrganizationName("Inviwo Foundation");
    QCoreApplication::setOrganizationDomain("inviwo.org");
    QCoreApplication::setApplicationName(displayName.c_str());
    fileWatcher_ = new QFileSystemWatcher(this);
    connect(fileWatcher_, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
}

InviwoApplicationQt::~InviwoApplicationQt() {}

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

    if (!fileWatcher_->files().contains(qFileName))
        fileWatcher_->addPath(qFileName);
}

void InviwoApplicationQt::stopFileObservation(std::string fileName) {
    QString qFileName = QString::fromStdString(fileName);

    if (fileWatcher_->files().contains(qFileName))
        fileWatcher_->removePath(qFileName);
}

void InviwoApplicationQt::fileChanged(QString fileName) {
    wait(200);

    if (QFile::exists(fileName)) {
        std::string fileNameStd = fileName.toLocal8Bit().constData();

        for (size_t i=0; i<fileObservers_.size(); i++) {
            std::vector<std::string> observedFiles = fileObservers_[i]->getFiles();

            if (std::find(observedFiles.begin(), observedFiles.end(), fileNameStd) != observedFiles.end())
                fileObservers_[i]->fileChanged(fileNameStd);
        }
    }
}

void InviwoApplicationQt::closeInviwoApplication() {
    QCoreApplication::quit();
}

void InviwoApplicationQt::playSound(unsigned int soundID) {
    // Qt currently does not support playing sounds from resources
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    if ((dynamic_cast<BoolProperty*>
         (InviwoApplication::getPtr()->getSettingsByType<SystemSettings>()->getPropertyByIdentifier("enableSound"))->get())) {
        if (soundID == IVW_OK) QSound::play(QString::fromStdString(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_RESOURCES)
                                                +"sounds/ok.wav"));
        else if (soundID == IVW_ERROR) QSound::play(QString::fromStdString(InviwoApplication::getPtr()->getPath(
                        InviwoApplication::PATH_RESOURCES)+"sounds/error.wav"));
    }

#endif
}


void InviwoApplicationQt::initialize(registerModuleFuncPtr regModuleFunc) {
    LogInfoCustom("InviwoInfo", "Qt Version " << QT_VERSION_STR);
    InviwoApplication::initialize(regModuleFunc);
    // Since QtWidgets are not a module we have to register it our self
    InviwoModule* module = new QtWidgetModule();
    registerModule(module);
    module->initialize();
}

Timer* InviwoApplicationQt::createTimer()const {
    return new TimerQt();
}

void InviwoApplicationQt::wait(int ms) {
    if (ms <= 0)
        return;

#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

} // namespace
