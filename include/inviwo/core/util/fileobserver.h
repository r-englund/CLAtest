#ifndef IVW_FILEOBSERVER_H
#define IVW_FILEOBSERVER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <string>
#include <vector>

namespace inviwo {

class IVW_CORE_API FileObserver {

public:
    void startFileObservation(std::string fileName);
    void stopFileObservation(std::string fileName);
    std::vector<std::string> getFiles();

    bool isObserved(std::string fileName);
    void increaseNumObservers(std::string fileName);
    void decreaseNumObservers(std::string fileName);
    int getNumObservers(std::string fileName);

    virtual void fileChanged(std::string fileName) = 0;

private:
    std::vector<std::pair<std::string, int> > observedFiles_; ///< stores the files to be observed
                                                              ///< plus the number of observers for each
};

} // namespace

#endif // IVW_FILEOBSERVER_H
