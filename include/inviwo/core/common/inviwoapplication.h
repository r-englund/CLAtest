#ifndef IVW_INVIWOAPPLICATION_H
#define IVW_INVIWOAPPLICATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwocore.h>
#include <inviwo/core/common/inviwomodule.h>
#include <inviwo/core/network/processornetwork.h>
#include <inviwo/core/util/fileobserver.h>
#include <inviwo/core/util/settings.h>
#include <inviwo/core/util/singleton.h>

#include <inviwo/core/util/commandlineparser.h>

namespace inviwo {

class IVW_CORE_API InviwoApplication : public Singleton<InviwoApplication>, public VoidObserver {

public:
    InviwoApplication(std::string displayName, std::string basePath);
    InviwoApplication(int argc, char** argv, std::string displayName, std::string basePath);
    virtual ~InviwoApplication();

    virtual void initialize();
    virtual void deinitialize();

    enum PathType {
        PATH_DATA,
        PATH_MODULES,
        PATH_PROJECT
    };
    std::string getPath(PathType pathType, const std::string& suffix = "");

    void registerModule(InviwoModule* module) { modules_.push_back(module); }
    const std::vector<InviwoModule*> getModules() const { return modules_; }

    void setProcessorNetwork(ProcessorNetwork* processorNetwork) {
        processorNetwork_ = processorNetwork;
        addObservation(processorNetwork_);
        processorNetwork_->addObserver(this);             

    }
    ProcessorNetwork* getProcessorNetwork() { return processorNetwork_; }
    Settings* getSettings() { return settings_; }
    const CommandLineParser* getCommandLineParser() const { return commandLineParser_; } 

    virtual void registerFileObserver(FileObserver* fileObserver) { LogWarn("This Inviwo application does not support FileObservers."); }
    virtual void startFileObservation(std::string fileName) { LogWarn("This Inviwo application does not support FileObservers."); }
    virtual void stopFileObservation(std::string fileName) { LogWarn("This Inviwo application does not support FileObservers."); }

    enum MessageType {
        IVW_OK,
        IVW_ERROR
    };
    virtual void playSound(unsigned int soundID) { /*LogWarn("This Inviwo application does not support sound feedback.");*/ }

    bool hasNetworkChanged() const { return networkChanged_; }
    void setNetworkChanged(bool networkChanged) { networkChanged_ = networkChanged; }

protected:
    void printApplicationInfo();

    bool networkChanged_;

private:
    std::string displayName_;

    std::string basePath_;
 
    std::vector<InviwoModule*> modules_;

    ProcessorNetwork* processorNetwork_;

    CommandLineParser *commandLineParser_;

    Settings* settings_;

    bool initialized_;
};

} // namespace

#endif // IVW_INVIWOAPPLICATION_H
