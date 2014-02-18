/**********************************************************************
 * Copyright (C) 2013 Vistinct AB
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

#include "cvie2dprocessor.h"
#include <inviwo/core/datastructures/image/layerram.h>

namespace inviwo {

ProcessorClassName(CVIE2DProcessor, "CVIE2DProcessor"); 
ProcessorCategory(CVIE2DProcessor, "Context Vision");
ProcessorCodeState(CVIE2DProcessor, CODE_STATE_EXPERIMENTAL);

#define EndProcessIfFalse(success) if(!success){ passthrough(); return; }

CVIE2DProcessor::CVIE2DProcessor()
    : Processor(),
    inport_("image.inport"),
    outport_("image.outport"),
    enabled_("enabled", "Filtering Enabled", false),
    parameterFileDirectory_("parameterFileDirectory", "Parameter file directory", InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+"cvie/ext/CVIESDK/par"),
    findParameterFiles_("findParameterFiles", "Find parameter files"),
    parameterFile_("parameterFile", "Selected parameter file"),
    parameterSetting_("parameterSetting", "Parameter setting", 0)

{
    addPort(inport_);
    addPort(outport_);

    addProperty(enabled_);

    addProperty(parameterFileDirectory_);

    findParameterFiles_.onChange(this, &CVIE2DProcessor::findParameterFiles);
    addProperty(findParameterFiles_);

    parameterFile_.onChange(this, &CVIE2DProcessor::updateParameterFile);
    addProperty(parameterFile_);

    //addProperty(parameterSetting_);

    cvieContextCreated_ = false;
}

CVIE2DProcessor::~CVIE2DProcessor() {}

void CVIE2DProcessor::initialize() {
    Processor::initialize();
    createCVIEInstance();
}

void CVIE2DProcessor::deinitialize() {
    Processor::deinitialize();
    destroyCVIEInstance();
}

void CVIE2DProcessor::process() {
    EndProcessIfFalse(enabled_.get());

    //Setup CVIE Enhancement
    EndProcessIfFalse(setupEnhancement());

    //Run CVIE Enhancement
    EndProcessIfFalse(runEnhancement());
}

void CVIE2DProcessor::passthrough(){
    outport_.setConstData(inport_.getData());
}

//Create CVIE Instance with Configuration 
bool CVIE2DProcessor::createCVIEInstance(){
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
void CVIE2DProcessor::destroyCVIEInstance(){
    if(cvieContextCreated_){
        CVIEDestroy(&cvieHandle_);
        cvieContextCreated_ = false;
    }
}

bool CVIE2DProcessor::setupEnhancement(){
    if(!cvieContextCreated_)
        createCVIEInstance();

    if(cvieContextCreated_){
        uvec2 imageSize = inport_.getData()->getDimension();
        ECVIE cvieError = CVIEEnhanceSetup(cvieHandle_, static_cast<int>(imageSize.x), static_cast<int>(imageSize.y), CVIE_DATA_U8, parameterSetting_.get(), 0);

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

bool CVIE2DProcessor::runEnhancement(){
    if(cvieContextCreated_){
        const LayerRAM* layerIn = inport_.getData()->getColorLayer()->getRepresentation<LayerRAM>();
        bool allocateNewimageume = (!outport_.hasData() ||  !outport_.isDataOwner() || inport_.getData()->getDimension() != outport_.getData()->getDimension());
        LayerRAM* layerOut;

        if(allocateNewimageume)
            layerOut = static_cast<LayerRAM*>(layerIn->clone());
        else
            layerOut = outport_.getData()->getColorLayer()->getEditableRepresentation<LayerRAM>();

        ECVIE cvieError = CVIEEnhanceNext(cvieHandle_, const_cast<void*>(layerIn->getData()), layerOut->getData(), 0);

        if (cvieError != CVIE_E_OK) {
            if(allocateNewimageume)
                delete layerOut;

            char errstr[512];
            LogError("Error in CVIEEnhance: " << CVEMGetLastError(cvieHandle_, errstr, sizeof(errstr)));
            destroyCVIEInstance();
            return false;
        }

        if(allocateNewimageume){
            outport_.setData(new Image());
            outport_.getData()->addColorLayer(new Layer(layerOut));
        }

        return true;
    }
    else
        return false;
}

void CVIE2DProcessor::updateParameterFile(){
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

void CVIE2DProcessor::findParameterFiles(){
    parameterFileDirectory_.updateDirectoryTree();
    std::vector<std::string> parameterFiles = parameterFileDirectory_.getFiles("*.gop");
    std::vector<std::string> parameterDisplayName;
    for (size_t i=0; i<parameterFiles.size(); i++){ 
        parameterDisplayName.push_back(URLParser::getFileNameWithExtension(parameterFiles[i]));
    }
    parameterFile_.replaceOptions(parameterDisplayName, parameterDisplayName, parameterFiles);
}

} // namespace
