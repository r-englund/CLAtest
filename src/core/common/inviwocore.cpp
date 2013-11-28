#include <inviwo/core/common/inviwocore.h>

//Data Structures
#include <inviwo/core/datastructures/volume/volumeramconverter.h>
#include <inviwo/core/datastructures/image/imageramconverter.h>

//Meta Data
#include <inviwo/core/metadata/processormetadata.h>
#include <inviwo/core/metadata/processorwidgetmetadata.h>

//Utilizes
#include <inviwo/core/util/formatconversion.h>
#include <inviwo/core/util/systemcapabilities.h>
#include <inviwo/core/util/vectoroperations.h>

//Io
#include <inviwo/core/io/datvolumereader.h>
#include <inviwo/core/io/ivfvolumereader.h>

//Others
#include <inviwo/core/processors/canvasprocessor.h>
//Properties
#include <inviwo/core/properties/buttonproperty.h>

namespace inviwo {

InviwoCore::InviwoCore() : InviwoModule() {
    setIdentifier("Core");
    //setXMLFileName(InviwoApplication::app()->getRootPath() + "/src/core/core.xml", true);

    addRepresentationConverter(new VolumeDisk2RAMConverter());
    addRepresentationConverter(new ImageDisk2RAMConverter());

    addMetaData(new PositionMetaData());
    addMetaData(new ProcessorMetaData());
    addMetaData(new ProcessorWidgetMetaData());

    addCapabilities(new SystemCapabilities());

    addDataReader(new DatVolumeReader());
    addDataReader(new IvfVolumeReader());

    allocTest_ = NULL;
}

void InviwoCore::setupModuleSettings(){
    if (getSettings()){

        OptionPropertyInt* viewMode_ = new OptionPropertyInt("viewMode","",0);
        viewMode_->addOption("developerMode","developerMode",0);
        viewMode_->addOption("applicationMode","applicationMode",1);
        getSettings()->addProperty(viewMode_);
        viewMode_->setVisibility(INVISIBLE);
        getSettings()->addProperty(new BoolProperty("txtEditor", "Use system text editor", true));

        getSettings()->addProperty(new BoolProperty("shaderReloading", "Automatically reload shaders", true));

        getSettings()->addProperty(new BoolProperty("enablePortInspectors", "Enable port inspectors", true));

        getSettings()->addProperty(new BoolProperty("enableSound", "Enable sound", true));

        getSettings()->addProperty(new BoolProperty("displayLinks", "Display links", false));

        getSettings()->addProperty(new IntProperty("useRAMPercent", "Max Use Mem %", 50, 1, 100));

        ButtonProperty* btnAllocTest = new ButtonProperty("allocTest", "Perform Allocation Test");
        btnAllocTest->registerClassMemberFunction(this, &InviwoCore::allocationTest);
        getSettings()->addProperty(btnAllocTest);

        SystemCapabilities* sysInfo = getTypeFromVector<SystemCapabilities>(getCapabilities());
        if (sysInfo){
            ButtonProperty* btnSysInfo = new ButtonProperty("printSysInfo", "Print System Info");
            btnSysInfo->registerClassMemberFunction(sysInfo, &SystemCapabilities::printInfo);
            getSettings()->addProperty(btnSysInfo);  
        }           
    }
}

void InviwoCore::allocationTest(){
    if (getSettings()){
        SystemCapabilities* sysInfo = getTypeFromVector<SystemCapabilities>(getCapabilities());
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
