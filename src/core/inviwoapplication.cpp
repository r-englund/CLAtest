#include <inviwo/core/inviwoapplication.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/metadata/metadatafactory.h>
#include <inviwo/core/datastructures/representationconverterfactory.h>
#include <inviwo/core/util/systeminfo.h>
#include <modules/moduleregistration.h>

#include <inviwo/core/properties/buttonproperty.h>

namespace inviwo {

const std::string InviwoApplication::logSource_ = "InviwoApplication";


InviwoApplication::InviwoApplication(std::string displayName, std::string basePath)
                                     : displayName_(displayName), basePath_(basePath)
{
    init(this);
    allocTest_ = NULL;
}

InviwoApplication::~InviwoApplication() {}

void InviwoApplication::initialize() {
    resourcesInformation_ = new ResourceInfoContainer();
    resourcesInformation_->addInfo(new SystemInfo());

    settings_ = new Settings();
    settings_->initialize();

    registerModule(new InviwoCore());
    registerAllModules(this);
    for (size_t i=0; i<modules_.size(); i++)
        modules_[i]->initialize();

    resourcesInformation_->printInfos();

    setSystemSettings();

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

void InviwoApplication::setSystemSettings(){
    if(settings_){
        SystemInfo* sysInfo = getResourceInfo()->getInfo<SystemInfo>();
        if(sysInfo){
            ButtonProperty* btnSysInfo = new ButtonProperty("printSysInfo", "Print System Info");
            btnSysInfo->registerClassMemberFunction(sysInfo, &SystemInfo::printInfo);
            settings_->addProperty(btnSysInfo);
            
            settings_->addProperty(new IntProperty("useRAMPercent", "Max Use Mem %", 50, 1, 100));

            ButtonProperty* btnAllocTest = new ButtonProperty("allocTest", "Perform Allocation Test");
            btnAllocTest->registerClassMemberFunction(this, &InviwoApplication::allocationTest);
            settings_->addProperty(btnAllocTest);
        }
    }
}

void InviwoApplication::allocationTest(){
    if(settings_){
        SystemInfo* sysInfo = getResourceInfo()->getInfo<SystemInfo>();
        if(sysInfo){
            if(allocTest_){
                delete allocTest_;
                LogInfo("Deleted previous test allocation");
            }
            IntProperty* useRAMPercent = dynamic_cast<IntProperty*>(settings_->getPropertyByIdentifier("useRAMPercent"));
            size_t memBytesAlloc = sysInfo->getAvailableMemory()*1024*1024; //In Bytes
            LogInfo("Maximum Available Memory is " << memBytesAlloc/(1024*1024) << " MB");
            memBytesAlloc /= 100; //1% of total available memory
            memBytesAlloc *= useRAMPercent->get(); //?% of total available memory
            try
            {
                allocTest_ = new uint8_t[memBytesAlloc];
                LogInfo("Allocated " << memBytesAlloc/(1024*1024) << " MB, which is " << useRAMPercent->get() << "% of available memory");
            }
            catch(std::bad_alloc&)
            {
                LogError("Failed allocation of " << memBytesAlloc/(1024*1024) << " MB, which is " << useRAMPercent->get() << "% of available memory");
            }
        }
    }
}

} // namespace
