/**********************************************************************
 * Copyright (C) 2014 ContextVision AB
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

#include "cvscprocessor.h"
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo {

ProcessorClassName(CVSCProcessor, "CVSCProcessor"); 
ProcessorCategory(CVSCProcessor, "Context Vision");
ProcessorCodeState(CVSCProcessor, CODE_STATE_EXPERIMENTAL);

#define EndProcessIfFalse(success) if(!success){ passthrough(); return; }

CVSCProcessor::CVSCProcessor()
    : Processor(),
    inport_("volume.inport"),
    outport_("volume.outport"),
    enabled_("enabled", "Filtering Enabled", false),
    parameterFile_("parameterFile", "Parameter file", InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_VOLUMES)),
    cvscInit_(false)
{
    addPort(inport_);
    addPort(outport_);

    addProperty(enabled_);

    parameterFile_.onChange(this, &CVSCProcessor::updateParameterFile);
    addProperty(parameterFile_);
}

CVSCProcessor::~CVSCProcessor() {}

void CVSCProcessor::initialize() {
    Processor::initialize();
    updateParameterFile();
}

void CVSCProcessor::deinitialize() {
    Processor::deinitialize();
    destroyCVSCInstance();
}

void CVSCProcessor::process() {
    EndProcessIfFalse(enabled_.get());

    //Setup CVSC
    EndProcessIfFalse(setupScanConversion());

    //Run CVSC
    EndProcessIfFalse(runScanConversion());
}

void CVSCProcessor::passthrough(){
    outport_.setConstData(inport_.getData());
}

//Create CVSC Instance
bool CVSCProcessor::initCVSCInstance(){
    if(!cvscInit_){
        int result = CVSC_Init();
        cvscInit_ = true;

        if (result != CVSC_OK) {
            LogError("CVSC_CalcConvertedSize failed with error " << result << ".");
            destroyCVSCInstance();
            return false;
        }
    }

    return true;
}

//Destroy CVSC Instance
void CVSCProcessor::destroyCVSCInstance(){
    if(cvscInit_){
        CVSC_Destroy();
        cvscInit_ = false;
    }
}

bool CVSCProcessor::setupScanConversion(){
    if(!cvscInit_)
        initCVSCInstance();

    if(cvscInit_){
        uvec3 volSize = inport_.getData()->getDimension();

        CVSC_DataType dataType;
        switch (inport_.getData()->getDataFormat()->getId())
        {
        case UINT8:
            dataType = U8;
            break;
        default:
            LogError("Error in setupScanConversion: DataFormat " << inport_.getData()->getDataFormat()->getString() << " unsupported");
            destroyCVSCInstance();
            return false;
        }

        int result = CVSC_CalcConvertedSize(&params_, &outVolume_.width, &outVolume_.height, &outVolume_.depth);
        outVolume_.type = CVSC_DataType::U8;

        if(result != CVSC_OK) {
            LogError("CVSC_CalcConvertedSize failed with error " << result << ".");
            destroyCVSCInstance();
            return false;
        }

        bool allocateNewVolume = (!outport_.hasData() ||  !outport_.isDataOwner());
        uvec3 outDim = uvec3(static_cast<unsigned int>(outVolume_.width), static_cast<unsigned int>(outVolume_.height), static_cast<unsigned int>(outVolume_.depth));
        if(!allocateNewVolume){
            if(outDim != outport_.getData()->getDimension())
                allocateNewVolume = true;
        }

        if(allocateNewVolume){
            outport_.setData(new Volume(outDim, DataUINT8::get()));
        }

        return true;
    }
    else
        return false;
}

bool CVSCProcessor::runScanConversion(){
    if(cvscInit_){
        const VolumeRAM* volIn = inport_.getData()->getRepresentation<VolumeRAM>();
        CVSC_Volume inVolume;
        inVolume.width = static_cast<int>(volIn->getDimension().x);
        inVolume.height = static_cast<int>(volIn->getDimension().y);
        inVolume.depth = static_cast<int>(volIn->getDimension().z);
        inVolume.type = outVolume_.type;
        inVolume.data = const_cast<void*>(volIn->getData());

        VolumeRAM* volOut = outport_.getData()->getEditableRepresentation<VolumeRAM>();
        outVolume_.data = volOut->getData();

        int result = CVSC_ScanConvert(&inVolume, &outVolume_, &params_);

        if (result != CVSC_OK) {
            LogError("CVSC_ScanConvert failed with error " << result << ".");
            destroyCVSCInstance();
            return false;
        }

        return true;
    }
    else
        return false;
}

void CVSCProcessor::updateParameterFile(){
    //if(URLParser::fileExists(parameterFile_.get().c_str())){
        params_.ScanDepth = 69.9417f;
        params_.TransRadius = 77.5694f;
        params_.TransAngleSpan = glm::degrees(1.0085f);
        params_.TiltRadius  = 57.5994f;
        params_.TiltAngleSpan  = glm::degrees(0.9638f);
        params_.VoxelSize    = 0.25f;

        LogInfo("Scan Depth           (mm) : " << params_.ScanDepth);
        LogInfo("Transducer Radius    (mm) : " << params_.TransRadius)
        LogInfo("Transducer Span (degrees) : " << params_.TransAngleSpan)
        LogInfo("Tilt Radius          (mm) : " << params_.TiltRadius)
        LogInfo("Tilt Span       (degrees) : " << params_.TiltAngleSpan)
        LogInfo("Voxel Size           (mm) : " << params_.VoxelSize)
   // }
}

} // namespace
