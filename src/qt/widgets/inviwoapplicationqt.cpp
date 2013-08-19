#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
#include <QFile>
#include <QFileInfo>
#include <QSound>

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
    connect(fileWatcher_, SIGNAL(directoryChanged(QString)), this, SLOT(directoryChanged(QString)));
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
        QFileInfo info(qFileName);
        QString path = info.absolutePath();
        if(!fileWatcher_->directories().contains(path)){
            fileWatcher_->addPath(path);
        }
    }
}

void InviwoApplicationQt::stopFileObservation(std::string fileName) {
    QString qFileName = QString::fromStdString(fileName);
    ivwAssert(fileWatcher_->files().contains(qFileName),
              "trying to stop observation of an unobserved file: "+fileName);
    if (fileWatcher_->files().contains(qFileName)){
        fileWatcher_->removePath(qFileName);
        QFileInfo info(qFileName);
        QString path = info.absolutePath();
        QStringList result = fileWatcher_->files().filter(path);
        if(result.isEmpty()){
            fileWatcher_->removePath(path);
        }
    }
}

void InviwoApplicationQt::directoryChanged(QString dirName){
    if(!filesChanged_.isEmpty()){
        if(QFile::exists(filesChanged_.first())){
            std::string fileNameStd = filesChanged_.first().toLocal8Bit().constData();
            for (size_t i=0; i<fileObservers_.size(); i++) {
                std::vector<std::string> observedFiles = fileObservers_[i]->getFiles();
                if (std::find(observedFiles.begin(), observedFiles.end(), fileNameStd) != observedFiles.end())
                    fileObservers_[i]->fileChanged(fileNameStd);
            }
        }
        filesChanged_.removeAt(0);
    }
}

void InviwoApplicationQt::fileChanged(QString fileName) {
    filesChanged_.append(fileName);
}

void InviwoApplicationQt::playSound(unsigned int soundID) {
    // Qt currently does not support playing sounds from resources
    if((dynamic_cast<BoolProperty*>(InviwoApplication::getPtr()->getSettings()->getPropertyByIdentifier("soundsOn"))->get())){
        if (soundID == IVW_OK) QSound::play(QString::fromStdString(IVW_DIR+"resources/sounds/ok.wav"));
        else if (soundID == IVW_ERROR) QSound::play(QString::fromStdString(IVW_DIR+"resources/sounds/error.wav"));
    }
}

} // namespace
