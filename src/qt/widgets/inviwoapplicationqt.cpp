#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
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

InviwoApplicationQt::InviwoApplicationQt(std::string displayName, std::string basePath,
                                         int& argc, char** argv)
                                     : QApplication(argc, argv),
                                       InviwoApplication(argc, argv, displayName, basePath)  
{
    QCoreApplication::setOrganizationName("Inviwo");
    //QCoreApplication::setOrganizationDomain("inviwo.org");
    QCoreApplication::setApplicationName(displayName.c_str());

    // initialize singletons
    //ProcessorWidgetFactory::init();
    PropertyWidgetFactoryQt::init();

    fileWatcher_ = new QFileSystemWatcher();
    connect(fileWatcher_, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
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

void InviwoApplicationQt::playSound(unsigned int soundID) {
    // Qt currently does not support playing sounds from resources
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    if((dynamic_cast<BoolProperty*>(InviwoApplication::getPtr()->getSettings()->getPropertyByIdentifier("soundsOn"))->get())){
        if (soundID == IVW_OK) QSound::play(QString::fromStdString(IVW_DIR+"resources/sounds/ok.wav"));
        else if (soundID == IVW_ERROR) QSound::play(QString::fromStdString(IVW_DIR+"resources/sounds/error.wav"));
    }
#endif
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
