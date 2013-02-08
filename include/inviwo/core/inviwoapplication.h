#ifndef IVW_INVIWOAPPLICATION_H
#define IVW_INVIWOAPPLICATION_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/inviwocore.h>
#include <inviwo/core/inviwomodule.h>
#include <inviwo/core/network/processornetwork.h>
#include <inviwo/core/util/fileobserver.h>
#include <inviwo/core/util/resourceinfocontainer.h>
#include <inviwo/core/util/settings.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_CORE_API InviwoApplication : public Singleton<InviwoApplication> {

public:
    InviwoApplication(std::string displayName_, std::string basePath_);
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

    void setProcessorNetwork(ProcessorNetwork* processorNetwork) { processorNetwork_ = processorNetwork; }
    ProcessorNetwork* getProcessorNetwork() { return processorNetwork_; }
    ResourceInfoContainer* getResourceInfoContainer() { return resourcesInformation_; }
    Settings* getSettings() { return settings_; }

    virtual void registerFileObserver(FileObserver* fileObserver) { LogWarn("This Inviwo application does not support FileObservers."); }
    virtual void startFileObservation(std::string fileName) { LogWarn("This Inviwo application does not support FileObservers."); }
    virtual void stopFileObservation(std::string fileName) { LogWarn("This Inviwo application does not support FileObservers."); }

    enum MessageType {
        IVW_OK,
        IVW_ERROR
    };
    virtual void playSound(unsigned int soundID) { /*LogWarn("This Inviwo application does not support sound feedback.");*/ }

protected:
    void setSystemSettings();
    void allocationTest();

private:
    std::string displayName_;

    std::string basePath_;
 
    std::vector<InviwoModule*> modules_;

    ProcessorNetwork* processorNetwork_;

    ResourceInfoContainer* resourcesInformation_;

    Settings* settings_;

    bool initialized_;

    uint32_t* allocTest_;

    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_INVIWOAPPLICATION_H
