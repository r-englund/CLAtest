#include "cvie3dprocessor.h"
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo {

ProcessorClassName(CVIE3DProcessor, "CVIE3DProcessor"); 
ProcessorCategory(CVIE3DProcessor, "Context Vision");
ProcessorCodeState(CVIE3DProcessor, CODE_STATE_EXPERIMENTAL);

#define EndProcessIfFalse(success) if(!success){ passthrough(); return; }

CVIE3DProcessor::CVIE3DProcessor()
    : Processor(),
    inport_("volume.inport"),
    outport_("volume.outport"),
    enabled_("enabled", "Filtering Enabled", false),
    confFile_("confFile", "Configuration file", IVW_DIR+"modules/cvie3D/ext/CVIE3DSDK/bin/CVIE3D.conf"),
    parameterFile_("parameterFile", "Parameter file", IVW_DIR+"modules/cvie3D/ext/CVIE3DSDK/par/default.gop"),
    parameterSetting_("parameterSetting", "Parameter setting", 0)
{
    addPort(inport_);
    addPort(outport_);

    addProperty(enabled_);

    confFile_.onChange(this, &CVIE3DProcessor::updateConfigurationFile);
    addProperty(confFile_);

    parameterFile_.onChange(this, &CVIE3DProcessor::updateParameterFile);
    addProperty(parameterFile_);

    //addProperty(parameterSetting_);

    cvieContextCreated_ = false;
}

CVIE3DProcessor::~CVIE3DProcessor() {}

void CVIE3DProcessor::initialize() {
    Processor::initialize();
}

void CVIE3DProcessor::deinitialize() {
    Processor::deinitialize();
    destroyCVIE3DInstance();
}

void CVIE3DProcessor::process() {
    EndProcessIfFalse(enabled_.get());

    //Setup CVIE3D Enhancement
    EndProcessIfFalse(setupEnhancement());

    //Run CVIE3D Enhancement
    EndProcessIfFalse(runEnhancement());
}

void CVIE3DProcessor::passthrough(){
    outport_.setConstData(inport_.getData());
}

//Create CVIE3D Instance with Configuration 
bool CVIE3DProcessor::createCVIE3DInstance(){
    //LogInfo("configurationFile: " << confFile_.get());
    if(!cvieContextCreated_){
        ECVIE3D cvieError = CVIE3DCreate(&cvieHandle_, confFile_.get().c_str(), NULL);
        cvieContextCreated_ = true;

        if (cvieError != ECVIE3D_CudaInitializedOk && cvieError != ECVIE3D_CpuInitializedOk) {
            std::string msg;
            char errstr[512];
            if (cvieError == ECVIE3D_LicenseError) {
                msg = "License error.\n";
            }
            else {
                msg = CVEMGetLastError(cvieHandle_, errstr, sizeof(errstr));
            }

            LogError("Error in createCVIE3DInstance: " << msg.c_str());
            destroyCVIE3DInstance();
            return false;
        }
    }

    return true;
}

//Destroy CVIE3D Instance
void CVIE3DProcessor::destroyCVIE3DInstance(){
    if(cvieContextCreated_){
        CVIE3DDestroy(&cvieHandle_);
        cvieContextCreated_ = false;
    }
}

bool CVIE3DProcessor::setupEnhancement(){
    if(cvieContextCreated_){
        uvec3 volSize = inport_.getData()->getDimension();
        ECVIE3D cvieError = CVIE3DSetupEnhance(cvieHandle_, static_cast<int>(volSize.x), static_cast<int>(volSize.y), static_cast<int>(volSize.z), CVIE3D_DATA_U8, CVIE3D_Volume, parameterSetting_.get());

        if (cvieError != ECVIE3D_Ok) {
            char errstr[512];
            LogError("Error in CVIE3DSetupEnhance: " << CVEMGetLastError(cvieHandle_, errstr, sizeof(errstr)));
            destroyCVIE3DInstance();
            return false;
        }

        return true;
    }
    else
        return false;
}

bool CVIE3DProcessor::runEnhancement(){
    if(cvieContextCreated_){
        const VolumeRAM* volIn = inport_.getData()->getRepresentation<VolumeRAM>();
        bool allocateNewVolume = (!outport_.hasData() ||  !outport_.isDataOwner() || inport_.getData()->getDimension() != outport_.getData()->getDimension());
        VolumeRAM* volOut;

        if(allocateNewVolume)
            volOut = static_cast<VolumeRAM*>(volIn->clone());
        else
            volOut = outport_.getData()->getEditableRepresentation<VolumeRAM>();

        ECVIE3D cvieError = CVIE3DEnhance(cvieHandle_, const_cast<void*>(volIn->getData()), volOut->getData(), 0);

        if (cvieError != ECVIE3D_Ok) {
            if(allocateNewVolume)
                delete volOut;

            char errstr[512];
            LogError("Error in CVIE3DEnhance: " << CVEMGetLastError(cvieHandle_, errstr, sizeof(errstr)));
            destroyCVIE3DInstance();
            return false;
        }

        if(allocateNewVolume)
            outport_.setData(new Volume(volOut));

        return true;
    }
    else
        return false;
}

void CVIE3DProcessor::updateConfigurationFile(){
    destroyCVIE3DInstance();
    createCVIE3DInstance();
    updateParameterFile();
}

void CVIE3DProcessor::updateParameterFile(){
    // Set parameter file
    int nSettings = 0;
    //LogInfo("parameterFile: " << parameterFile_.get());
    ECVIE3D cvieError = CVIE3DSetParameterFile(cvieHandle_, parameterFile_.get().c_str(), &nSettings);

    if (cvieError != ECVIE3D_Ok) {
        char errstr[512];
        LogError("Error in CVIE3DSetParameterFile: " << CVEMGetLastError(cvieHandle_, errstr, sizeof(errstr)));
        //destroyCVIE3DInstance();
    }
}

} // namespace
