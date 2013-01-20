#ifndef IVW_FILEOBSERVER_H
#define IVW_FILEOBSERVER_H

#include <string>
#include <vector>

namespace inviwo {

class FileObserver {

public:
    void startFileObservation(std::string fileName);
    void stopFileObservation(std::string fileName);
    std::vector<std::string> getFiles();

    virtual void fileChanged(std::string fileName) = 0;

private:
    std::vector<std::string> files_;
};

} // namespace

#endif // IVW_FILEOBSERVER_H
