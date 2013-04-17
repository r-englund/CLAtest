#include <inviwo/core/util/fileobserver.h>
#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

//TODO: this class is not thread safe

void FileObserver::startFileObservation(std::string fileName) {
    if (isObserved(fileName))
        increaseNumObservers(fileName);
    else {
        observedFiles_.push_back(std::pair<std::string,int>(fileName, 1));
        InviwoApplication::getRef().startFileObservation(fileName);
    }
}

void FileObserver::stopFileObservation(std::string fileName) {
    if (isObserved(fileName)) {
        if (getNumObservers(fileName) > 1)
            decreaseNumObservers(fileName);
        else {
            observedFiles_.erase(std::remove(observedFiles_.begin(), observedFiles_.end(), std::pair<std::string,int>(fileName,getNumObservers(fileName))), observedFiles_.end());
            InviwoApplication::getRef().stopFileObservation(fileName);
        }
    }
    /*
    for (size_t i=0;i<observedFiles_.size();i++)
        std::cout << observedFiles_[i].first << ":" << observedFiles_[i].second << std::endl;
    std::cout << std::endl;
    */
}

void FileObserver::increaseNumObservers(std::string fileName) {
    for (size_t i=0;i<observedFiles_.size();i++)
        if (observedFiles_[i].first == fileName)
            observedFiles_[i].second++;
}

void FileObserver::decreaseNumObservers(std::string fileName) {
    for (size_t i=0;i<observedFiles_.size();i++)
        if (observedFiles_[i].first == fileName)
            observedFiles_[i].second--;
}

int FileObserver::getNumObservers(std::string fileName) {
    for (size_t i=0;i<observedFiles_.size();i++)
        if (observedFiles_[i].first == fileName)
            return observedFiles_[i].second;
    return 0;
}

bool FileObserver::isObserved(std::string fileName) {
    return (getNumObservers(fileName) > 0);
}

std::vector<std::string> FileObserver::getFiles() {
    std::vector<std::string> files;
    for (size_t i=0;i<observedFiles_.size();i++)
        files.push_back(observedFiles_[i].first);
    return files;
}

} // namespace