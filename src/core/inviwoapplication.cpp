#include <inviwo/core/inviwoapplication.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/metadata/metadatafactory.h>
#include <inviwo/core/datastructures/representationconverterfactory.h>
#include <inviwo/core/util/systeminfo.h>
#include <modules/moduleregistration.h>

namespace inviwo {

const std::string InviwoApplication::logSource_ = "InviwoApplication";


InviwoApplication::InviwoApplication(std::string displayName, std::string basePath)
                                     : displayName_(displayName), basePath_(basePath)
{
    init(this);
}

InviwoApplication::~InviwoApplication() {}

void InviwoApplication::initialize() {
    printSystemInfo();

    settings_ = new Settings();
    settings_->initialize();
    settings_->addProperty(new IntProperty("totalRAM", "Total RAM", 100, 1, 8192));

    registerModule(new InviwoCore());
    registerAllModules(this);
    for (size_t i=0; i<modules_.size(); i++)
        modules_[i]->initialize();

    // initialize singleton factories
    ProcessorFactory::init();
    MetaDataFactory::init();
    RepresentationConverterFactory::init();

    initialized_ = true;
}

void InviwoApplication::deinitialize() {
    for (size_t i=0; i<modules_.size(); i++)
        modules_[i]->deinitialize();
    initialized_ = false;
}

std::string InviwoApplication::getPath(PathType pathType, const std::string& suffix) {
    std::string result = basePath_ + "/";
    if (pathType == InviwoApplication::PATH_PROJECT)
        result += "data/workspaces/";
    else if (pathType == InviwoApplication::PATH_DATA)
        result += "data/volumes/";
    else if (pathType == InviwoApplication::PATH_MODULES)
        result += "modules/";
    result += suffix;
    return result;
}

} // namespace
