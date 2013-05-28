#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/processors/processorwidgetfactory.h>
#include <inviwo/core/metadata/metadatafactory.h>
#include <inviwo/core/datastructures/representationconverterfactory.h>
#include <modules/moduleregistration.h>

namespace inviwo {

InviwoApplication::InviwoApplication(std::string displayName, std::string basePath)
                                     : displayName_(displayName), basePath_(basePath)
{
    init(this);
}

InviwoApplication::InviwoApplication(int argc, char** argv, std::string displayName, std::string basePath)
                                    : displayName_(displayName), basePath_(basePath)
{
    commandLineParser_ = new CommandLineParser(argc, argv);
    init(this);
}

InviwoApplication::~InviwoApplication() {}

void InviwoApplication::initialize() {

    commandLineParser_->initialize();
    commandLineParser_->parse();
    settings_ = new Settings();
    settings_->initialize();

    printApplicationInfo();

    registerModule(new InviwoCore());
    registerAllModules(this);
    for (size_t i=0; i<modules_.size(); i++){
        modules_[i]->initialize();
        modules_[i]->setGlobalSettings(settings_);
    }

    // initialize singleton factories
    ProcessorFactory::init();
    MetaDataFactory::init();
    ProcessorWidgetFactory::init();
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

void InviwoApplication::printApplicationInfo(){
    //TODO: Remove line below!!!!!!!!
    LogInfo(DataVec3UINT8::str() << ":BA-" << DataVec3UINT8::bitsAllocated() << ":ID-" << DataVec3UINT8::id());
    LogInfoCustom("InviwoInfo", "Version: " << IVW_VERSION);
    std::string config = "";
#ifdef CMAKE_GENERATOR
    config += std::string(CMAKE_GENERATOR);
#endif
#if defined(CMAKE_BUILD_TYPE)
    config += " [" + std::string(CMAKE_BUILD_TYPE) + "]";
#elif defined(CMAKE_INTDIR)
    config += " [" + std::string(CMAKE_INTDIR) + "]";
#endif
    if (config != "")
        LogInfoCustom("InviwoInfo", "Config: " << config);
}

} // namespace
