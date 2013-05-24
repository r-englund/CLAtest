#include <inviwo/core/common/inviwocore.h>

//Data Structures
#include <inviwo/core/datastructures/volumeramconverter.h>
#include <inviwo/core/datastructures/imageramconverter.h>

//Meta Data
#include <inviwo/core/metadata/processormetadata.h>
#include <inviwo/core/metadata/processorwidgetmetadata.h>

//Utilizes
#include <inviwo/core/util/formatconversion.h>
#include <inviwo/core/util/systeminfo.h>
#include <inviwo/core/util/vectoroperations.h>

//Others
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/processors/datasource/volumesource.h>

//Properties
#include <inviwo/core/properties/buttonproperty.h>

namespace inviwo {

InviwoCore::InviwoCore() : InviwoModule() {
    setIdentifier("Core");
    //setXMLFileName(InviwoApplication::app()->getRootPath() + "/src/core/core.xml", true);

    registerProcessor(VolumeSource);

    addRepresentationConverter(new VolumeDisk2RAMConverter());
    addRepresentationConverter(new ImageDisk2RAMConverter());

    addMetaData(new PositionMetaData());
    addMetaData(new ProcessorMetaData());
    addMetaData(new ProcessorWidgetMetaData());

    addResourceInfo(new SystemInfo());

    allocTest_ = NULL;
}

void InviwoCore::setupModuleSettings(){
    if (getSettings()){
        SystemInfo* sysInfo = getTypeFromVector<SystemInfo>(getResourceInfos());
        if (sysInfo){
            ButtonProperty* btnSysInfo = new ButtonProperty("printSysInfo", "Print System Info");
            btnSysInfo->registerClassMemberFunction(sysInfo, &SystemInfo::printInfo);
            getSettings()->addProperty(btnSysInfo);

            getSettings()->addProperty(new IntProperty("useRAMPercent", "Max Use Mem %", 50, 1, 100));

            ButtonProperty* btnAllocTest = new ButtonProperty("allocTest", "Perform Allocation Test");
            btnAllocTest->registerClassMemberFunction(this, &InviwoCore::allocationTest);
            getSettings()->addProperty(btnAllocTest);

            getSettings()->addProperty(new BoolProperty("txtEditor", "Use system text editor", true));

            getSettings()->addProperty(new BoolProperty("shaderReloading", "Automatically reload shaders", true));
        }           
    }
}

void InviwoCore::allocationTest(){
    if (getSettings()){
        SystemInfo* sysInfo = getTypeFromVector<SystemInfo>(getResourceInfos());
        if (sysInfo){
            if (allocTest_){
                delete allocTest_;
                LogInfo("Deleted previous test allocation");
            }
            IntProperty* useRAMPercent = dynamic_cast<IntProperty*>(getSettings()->getPropertyByIdentifier("useRAMPercent"));
            uint64_t memBytesAlloc = sysInfo->getAvailableMemory(); //In Bytes
            LogInfo("Maximum Available Memory is " << formatBytesToString(memBytesAlloc));
            memBytesAlloc /= 100; //1% of total available memory
            memBytesAlloc *= useRAMPercent->get(); //?% of total available memory
            try
            {
                allocTest_ = new uint32_t[static_cast<uint32_t>(memBytesAlloc/4)];
                LogInfo("Allocated " << formatBytesToString(memBytesAlloc) << ", which is " << useRAMPercent->get() << "% of available memory");
            }
            catch(std::bad_alloc&)
            {
                LogError("Failed allocation of " << formatBytesToString(memBytesAlloc) << ", which is " << useRAMPercent->get() << "% of available memory");
            }
        }
    }
}

} // namespace
