#ifndef IVW_INVIWOAPPLICATION_H
#define IVW_INVIWOAPPLICATION_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/inviwo.h"
#include "inviwo/core/inviwocore.h"
#include "inviwo/core/inviwomodule.h"

namespace inviwo {

class IVW_CORE_API InviwoApplication {

public:
    InviwoApplication(std::string displayName_, std::string basePath_);
    virtual ~InviwoApplication();

    static InviwoApplication* app();

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

protected:
    static InviwoApplication* app_;

private:
    std::string displayName_;

    std::string basePath_;

    std::vector<InviwoModule*> modules_;

    bool initialized_;
};

} // namespace

#endif // IVW_INVIWOAPPLICATION_H
