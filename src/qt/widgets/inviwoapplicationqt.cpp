#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/qt/widgets/processors/processorwidgetfactoryqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>

#include <QSound>

namespace inviwo {

InviwoApplicationQt::InviwoApplicationQt(std::string displayName, std::string basePath,
                                         int& argc, char** argv)
                                     : QApplication(argc, argv),
                                       InviwoApplication(displayName, basePath)  
{
    QCoreApplication::setOrganizationName("Inviwo");
    //QCoreApplication::setOrganizationDomain("inviwo.org");
    QCoreApplication::setApplicationName(displayName.c_str());

    ProcessorWidgetFactoryQt::init();
    PropertyWidgetFactoryQt::init();
    reloadingFile_ = false;
    fileWatcher_ = new QFileSystemWatcher();
    connect(fileWatcher_, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
}

void InviwoApplicationQt::registerFileObserver(FileObserver* fileObserver) {
    ivwAssert(std::find(fileObservers_.begin(),fileObservers_.end(),fileObserver)==fileObservers_.end(),
              "File observer already registered.");
    fileObservers_.push_back(fileObserver);
}

void InviwoApplicationQt::startFileObservation(std::string fileName) {
    if (!fileWatcher_->files().contains(QString::fromStdString(fileName)))
        fileWatcher_->addPath(QString::fromStdString(fileName));
}

void InviwoApplicationQt::stopFileObservation(std::string fileName) {
    ivwAssert(fileWatcher_->files().contains(QString::fromStdString(fileName)),
              "trying to stop observation of an unobserved file: "+fileName);
    if (fileWatcher_->files().contains(QString::fromStdString(fileName)))
        fileWatcher_->removePath(QString::fromStdString(fileName));
}

void InviwoApplicationQt::fileChanged(QString fileName) {
    // FIXME: When using visual studio for file editing, the file is renamed, deleted, and copied over
    // therefore the filesystemwatcher might 'forget' about the file
    if (!reloadingFile_) {
        reloadingFile_ = true;
        std::string fileNameStd = fileName.toStdString();
        for (size_t i=0; i<fileObservers_.size(); i++) {
            std::vector<std::string> observedFiles = fileObservers_[i]->getFiles();
            if (std::find(observedFiles.begin(), observedFiles.end(), fileNameStd) != observedFiles.end())
                fileObservers_[i]->fileChanged(fileNameStd);
        }
        startFileObservation(fileNameStd);
        reloadingFile_ = false;
    }
}

void InviwoApplicationQt::playSound(unsigned int soundID) {
    // Qt currently does not support playing sounds from resources
    if (soundID == IVW_OK) QSound::play(QString::fromStdString(IVW_DIR+"resources/sounds/ok.wav"));
    else if (soundID == IVW_ERROR) QSound::play(QString::fromStdString(IVW_DIR+"resources/sounds/error.wav"));
}

} // namespace
