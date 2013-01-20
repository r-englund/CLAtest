#include "inviwo/qt/widgets/inviwoapplicationqt.h"
#include "inviwo/qt/widgets/properties/propertywidgetfactoryqt.h"

#include <QSound>

namespace inviwo {

InviwoApplicationQt::InviwoApplicationQt(std::string displayName, std::string basePath,
                                         int argc, char** argv)
                                     : InviwoApplication(displayName, basePath),
                                       QApplication(argc, argv)
{
    PropertyWidgetFactoryQt::init();
    fileWatcher_ = new QFileSystemWatcher();
    connect(fileWatcher_, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
}

void InviwoApplicationQt::registerFileObserver(FileObserver* fileObserver) {
    fileObservers_.push_back(fileObserver);
}

void InviwoApplicationQt::startFileObservation(std::string fileName) {
    fileWatcher_->addPath(QString::fromStdString(fileName));
}

void InviwoApplicationQt::stopFileObservation(std::string fileName) {
    fileWatcher_->removePath(QString::fromStdString(fileName));
}

void InviwoApplicationQt::fileChanged(QString fileName) {
    // FIXME: for some reason these slot is executed twice upon file change
    std::string fileNameStd = fileName.toStdString();
    for (size_t i=0; i<fileObservers_.size(); i++) {
        std::vector<std::string> observedFiles = fileObservers_[i]->getFiles();
        if (std::find(observedFiles.begin(), observedFiles.end(), fileNameStd) != observedFiles.end())
            fileObservers_[i]->fileChanged(fileNameStd);
    }
}

void InviwoApplicationQt::playSound(unsigned int soundID) {
    // Qt currently does not support playing sounds from resources
    if (soundID == IVW_OK) QSound::play(QString::fromStdString(IVW_DIR+"resources/sounds/ok.wav"));
    else if (soundID == IVW_ERROR) QSound::play(QString::fromStdString(IVW_DIR+"resources/sounds/error.wav"));
}

} // namespace
