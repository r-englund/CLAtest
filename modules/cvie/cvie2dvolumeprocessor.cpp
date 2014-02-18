/**********************************************************************
 * Copyright (C) 2104 Vistinct AB
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include "cvie2dvolumeprocessor.h"
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo {

ProcessorClassName(CVIE2DVolumeProcessor, "CVIE2DVolumeProcessor"); 
ProcessorCategory(CVIE2DVolumeProcessor, "Context Vision");
ProcessorCodeState(CVIE2DVolumeProcessor, CODE_STATE_EXPERIMENTAL);

#define EndProcessIfFalse(success) if(!success){ passthrough(); return; }

CVIE2DVolumeProcessor::CVIE2DVolumeProcessor()
    : Processor(),
    inport_("volume.inport"),
    outport_("volume.outport"),
    enabled_("enabled", "Filtering Enabled", false),
    onlyProcessSelectedSlice_("onlyProcessSelectedSlice", "Certain Slice Only", false),
    sliceNumber_("sliceNumber", "Slice Number", 1, 1, 256),
    parameterFileDirectory_("parameterFileDirectory", "Parameter file directory", InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+"cvie/ext/CVIESDK/par"),
    findParameterFiles_("findParameterFiles", "Find parameter files"),
    parameterFile_("parameterFile", "Selected parameter file"),
    parameterSetting_("parameterSetting", "Parameter setting", 0)

{
    addPort(inport_);
    addPort(outport_);

    addProperty(enabled_);
    
    onlyProcessSelectedSlice_.onChange(this, &CVIE2DVolumeProcessor::sliceOnlyChanged);
    addProperty(onlyProcessSelectedSlice_);

    addProperty(sliceNumber_);

    addProperty(parameterFileDirectory_);

    findParameterFiles_.onChange(this, &CVIE2DVolumeProcessor::findParameterFiles);
    addProperty(findParameterFiles_);

    parameterFile_.onChange(this, &CVIE2DVolumeProcessor::updateParameterFile);
    addProperty(parameterFile_);

    //addProperty(parameterSetting_);

    cvieContextCreated_ = false;
}

CVIE2DVolumeProcessor::~CVIE2DVolumeProcessor() {}

void CVIE2DVolumeProcessor::initialize() {
    Processor::initialize();
    createCVIEInstance();
}

void CVIE2DVolumeProcessor::deinitialize() {
    Processor::deinitialize();
    destroyCVIEInstance();
}

void CVIE2DVolumeProcessor::process() {
    int slices = static_cast<int>(inport_.getData()->getDimension().z);
    if (sliceNumber_.getMaxValue() != slices) {
        sliceNumber_.setMaxValue(slices);
    }

    EndProcessIfFalse(enabled_.get());

    //Setup CVIE Enhancement
    EndProcessIfFalse(setupEnhancement());

    //Run CVIE Enhancement
    EndProcessIfFalse(runEnhancement());
}

void CVIE2DVolumeProcessor::passthrough(){
    outport_.setConstData(inport_.getData());
}

//Create CVIE Instance with Configuration 
bool CVIE2DVolumeProcessor::createCVIEInstance(){
    //LogInfo("configurationFile: " << confFile_.get());
    if(!cvieContextCreated_){
        ECVIE cvieError = CVIECreate(&cvieHandle_, CVIE_CREATE_DEFAULT);
        cvieContextCreated_ = true;

        if (cvieError != CVIE_E_OK) {
            std::string msg;
            char errstr[512];
            if (cvieError == CVIE_E_LICENSE_ERROR) {
                msg = "License error.\n";
            }
            else {
                msg = CVEMGetLastError(cvieHandle_, errstr, sizeof(errstr));
            }

            LogError("Error in createCVIEInstance: " << msg.c_str());
            destroyCVIEInstance();
            return false;
        }
    }

    return true;
}

//Destroy CVIE Instance
void CVIE2DVolumeProcessor::destroyCVIEInstance(){
    if(cvieContextCreated_){
        CVIEDestroy(&cvieHandle_);
        cvieContextCreated_ = false;
    }
}

bool CVIE2DVolumeProcessor::setupEnhancement(){
    if(!cvieContextCreated_)
        createCVIEInstance();

    if(cvieContextCreated_){
        uvec3 volSize = inport_.getData()->getDimension();

        bool dataTypeSupported = true;
        int dataType;
        switch (inport_.getData()->getDataFormat()->getId())
        {
        case UINT8:
            dataType = CVIE_DATA_U8;
            break;
        case UINT12:
        case UINT16:
            dataType = CVIE_DATA_U16;
            break;
        case INT12:
        case INT16:
            dataType = CVIE_DATA_S16;
            break;
        case FLOAT16:
        case FLOAT32:
            dataType = CVIE_DATA_F32;
            break;
        default:
            LogError("Error in CVIESetupEnhance: DataFormat " << inport_.getData()->getDataFormat()->getString() << " unsupported");
            dataTypeSupported = false;
        }

        ECVIE cvieError;
        if(dataTypeSupported)
            cvieError = CVIEEnhanceSetup(cvieHandle_, static_cast<int>(volSize.x), static_cast<int>(volSize.y), dataType, 0, 0);
        else
            cvieError = CVIE_E_NOT_SUPPORTED;

        if (cvieError != CVIE_E_OK) {
            char errstr[512];
            LogError("Error in CVIESetupEnhance: " << CVEMGetLastError(cvieHandle_, errstr, sizeof(errstr)));
            destroyCVIEInstance();
            return false;
        }

        return true;
    }
    else
        return false;
}

bool CVIE2DVolumeProcessor::runEnhancement(){
    if(cvieContextCreated_){
        const VolumeRAM* volIn = inport_.getData()->getRepresentation<VolumeRAM>();
        bool allocateNewVolume = (!outport_.hasData() ||  !outport_.isDataOwner() || inport_.getData()->getDimension() != outport_.getData()->getDimension());
        VolumeRAM* volOut;

        if(allocateNewVolume)
            volOut = static_cast<VolumeRAM*>(volIn->clone());
        else
            volOut = outport_.getData()->getEditableRepresentation<VolumeRAM>();

        const uvec3& dim = volOut->getDimension();
        size_t start = 0;
        size_t end = dim.z;

        if(onlyProcessSelectedSlice_.get()){
            end = static_cast<size_t>(sliceNumber_.get());
            start = end-1;
        }

        for(size_t z=start; z<end; ++z){
            size_t pos = z*dim.x*dim.y;
            ECVIE cvieError = CVIEEnhanceNext(cvieHandle_, const_cast<void*>(volIn->getData(pos)), volOut->getData(pos), 0);

            if (cvieError != CVIE_E_OK) {
                if(allocateNewVolume)
                    delete volOut;

                char errstr[512];
                LogError("Error in CVIEEnhance: " << CVEMGetLastError(cvieHandle_, errstr, sizeof(errstr)));
                destroyCVIEInstance();
                return false;
            }
        }

        if(allocateNewVolume){
            outport_.setData(new Volume(volOut));
        }

        return true;
    }
    else
        return false;
}

void CVIE2DVolumeProcessor::updateParameterFile(){
    if(!cvieContextCreated_)
        createCVIEInstance();

    if(cvieContextCreated_){
        // Set parameter file
        int nSettings = 0;
        //LogInfo("parameterFile: " << parameterFile_.get());
        std::string parameterFile = parameterFile_.get();
        if(URLParser::fileExists(parameterFile.c_str())){
            std::vector<char> parameterFileChar(parameterFile.begin(), parameterFile.end());
            parameterFileChar.push_back('\0');
            ECVIE cvieError = CVIESetParameterFile(cvieHandle_, &parameterFileChar[0], &nSettings);

            if (cvieError != CVIE_E_OK) {
                char errstr[512];
                LogError("Error in CVIESetParameterFile: " << CVEMGetLastError(cvieHandle_, errstr, sizeof(errstr)));
                //destroyCVIEInstance();
            }
        }
    }
}

void CVIE2DVolumeProcessor::findParameterFiles(){
    parameterFileDirectory_.updateDirectoryTree();
    std::vector<std::string> parameterFiles = parameterFileDirectory_.getFiles("*.gop");
    std::vector<std::string> parameterDisplayName;
    for (size_t i=0; i<parameterFiles.size(); i++){ 
        parameterDisplayName.push_back(URLParser::getFileNameWithExtension(parameterFiles[i]));
    }
    parameterFile_.replaceOptions(parameterDisplayName, parameterDisplayName, parameterFiles);
}

void CVIE2DVolumeProcessor::sliceOnlyChanged(){
    sliceNumber_.setVisible(onlyProcessSelectedSlice_.get());
}

} // namespace
