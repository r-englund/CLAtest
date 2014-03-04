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
    scanDepth_("scanDepth", "Scan Depth (mm)", 60.0f, 1.0f, 500.0f, 1.0f),
    transRadius_("transRadius", "Transducer Radius (mm)", 70.0f, 0.0f, 500.0f, 1.0f),
    transAngleSpan_("transAngleSpan", "Transducer Span (deg.)", 60.0f, 0.1f, 360.0f, 0.1f),
    tiltRadius_("tiltRadius", "Tilt Radius (mm)", 60.0f, 0.0f, 500.0f, 1.0f),
    tiltAngleSpan_("tiltAngleSpan", "Tilt Span (deg.)", 60.0f, 0.1f, 360.0f, 0.1f),
    voxelSize_("voxelSize", "Voxel Size (mm)", 1.0f, 0.01f, 10.0f, 0.01f),
    cvscInit_(false)
{
    addPort(inport_);
    addPort(outport_);

    enabled_.onChange(this, &CVSCProcessor::cvscEnabled);
    addProperty(enabled_);

    addProperty(scanDepth_);
    addProperty(transRadius_);
    addProperty(transAngleSpan_);
    addProperty(tiltRadius_);
    addProperty(tiltAngleSpan_);
    addProperty(voxelSize_);
}

CVSCProcessor::~CVSCProcessor() {}

void CVSCProcessor::initialize() {
    Processor::initialize();

    params_.ScanDepth = scanDepth_.get();
    params_.TransRadius = transRadius_.get();
    params_.TransAngleSpan = transAngleSpan_.get();
    params_.TiltRadius  = tiltRadius_.get();
    params_.TiltAngleSpan  = tiltAngleSpan_.get();
    params_.VoxelSize    = voxelSize_.get();
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

void CVSCProcessor::cvscEnabled(){
    scanDepth_.setReadOnly(enabled_.get());
    transRadius_.setReadOnly(enabled_.get());
    transAngleSpan_.setReadOnly(enabled_.get());
    tiltRadius_.setReadOnly(enabled_.get());
    tiltAngleSpan_.setReadOnly(enabled_.get());
    voxelSize_.setReadOnly(enabled_.get());

    params_.ScanDepth = scanDepth_.get();
    params_.TransRadius = transRadius_.get();
    params_.TransAngleSpan = transAngleSpan_.get();
    params_.TiltRadius  = tiltRadius_.get();
    params_.TiltAngleSpan  = tiltAngleSpan_.get();
    params_.VoxelSize    = voxelSize_.get();

    LogInfo("Scan Depth           (mm) : " << params_.ScanDepth);
    LogInfo("Transducer Radius    (mm) : " << params_.TransRadius)
    LogInfo("Transducer Span (degrees) : " << params_.TransAngleSpan)
    LogInfo("Tilt Radius          (mm) : " << params_.TiltRadius)
    LogInfo("Tilt Span       (degrees) : " << params_.TiltAngleSpan)
    LogInfo("Voxel Size           (mm) : " << params_.VoxelSize)
}

} // namespace
