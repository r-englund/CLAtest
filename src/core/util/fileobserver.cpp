#include "inviwo/core/inviwoapplication.h"
#include "inviwo/core/util/fileobserver.h"

namespace inviwo {

void FileObserver::startFileObservation(std::string fileName) {
    files_.push_back(fileName);
    InviwoApplication::getRef().startFileObservation(fileName);
}

void FileObserver::stopFileObservation(std::string fileName) {
    files_.erase(std::remove(files_.begin(), files_.end(), fileName), files_.end());
    InviwoApplication::getRef().stopFileObservation(fileName);
}

std::vector<std::string> FileObserver::getFiles() {
    return files_;
}

} // namespace