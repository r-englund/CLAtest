/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Contact: Sathish Kottravel
 *
 *********************************************************************************/

#include <modules/pypackages/processors/pyprocessorbase.h>
#include <inviwo/core/util/formats.h>
#include <inviwo/core/datastructures/buffer/bufferram.h>
#include <modules/pypackages/pyscriptrunner.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>
#include <inviwo/core/datastructures/image/layerramprecision.h>

namespace inviwo {

PyProcessorBase::PyProcessorBase()
    : Processor()
    , pythonScriptFile_("pythonScript", "Edit Script", 
                        InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES) +
                            "pypackages/scripts/pyavailablepackagesinfo.py", "script",
                        INVALID_OUTPUT, PropertySemantics("TextEditor"))
    , runScript_("runScript", "Run Script")
    , script_()
    , requiresRun_(false) {
    addProperty(pythonScriptFile_);
    pythonScriptFile_.onChange(this, &PyProcessorBase::loadPythonScriptFile);
    addProperty(runScript_);
    runScript_.onChange(this, &PyProcessorBase::onRunScriptButtonClicked);
}

PyProcessorBase::~PyProcessorBase() {}

void PyProcessorBase::initialize() {
    Processor::initialize();
}

void PyProcessorBase::deinitialize() {
    Processor::deinitialize();
}

void PyProcessorBase::process() {
    Processor::process();
    //derived class process function should not be called after this
    if (requiresRun_) {
        runScript();
        requiresRun_ = false;
    }
}

//General
void PyProcessorBase::onRunScriptButtonClicked() {  
    reloadScript();
}

void PyProcessorBase::reloadScript() {
    loadPythonScriptFile();
    requiresRun_ = true;
}

void PyProcessorBase::loadPythonScriptFile() {
    std::string scriptFileName = pythonScriptFile_.get();
    std::ifstream file(scriptFileName.c_str());
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    PyScriptRunner::getPtr()->setScript(text);
}

void PyProcessorBase::runScript() {
    PyScriptRunner::getPtr()->run(false);
    std::string retError = PyScriptRunner::getPtr()->getError();
    if (retError!="") {
        LogWarn(retError);
        return;
    }
    std::string status = PyScriptRunner::getPtr()->getStandardOutput();
    if (status!="") LogInfo(status);
}

Buffer* PyProcessorBase::convertLayerToBuffer(LayerRAM* layer) {
    ivec2 dim = layer->getDimension();
    const DataFormatBase* format = layer->getDataFormat();
    Buffer* buffer = new Buffer(dim.x*dim.y, format);
    BufferRAM* bufferRAM = 0;
    bufferRAM = createBufferRAM(dim.x*dim.y, format, COLOR_ATTRIB, STATIC);

#define DataFormatIdMacro(i) \
    if ( dynamic_cast< BufferRAMCustomPrecision<Data##i::type, Data##i::bits> *>(bufferRAM) ) { \
    dynamic_cast< BufferRAMCustomPrecision<Data##i::type, Data##i::bits> *>(bufferRAM)->initialize(layer->getData()); \
    LogWarn("Buffer format is " << format->getString()); }
#include <inviwo/core/util/formatsdefinefunc.h>

    buffer->addRepresentation(bufferRAM);
    return buffer;
}

 void PyProcessorBase::freeAll() {
     freeAllBuffers();
     freeAllLayers();
 }

///////////////////////////////////////////////////////////////////////////////
//Buffer management
bool PyProcessorBase::allocatePyBuffer(std::string bufferName, std::string bufferType, size_t bufferSize) {
    //allocate and cache buffer
    if (pyBufferMap_.find(bufferName)==pyBufferMap_.end()) {
        Buffer* buffer = 0;

        if (0) buffer = 0;
#define DataFormatIdMacro(i) else if (bufferType == Data##i::str()) buffer = new Buffer_##i();
#include <inviwo/core/util/formatsdefinefunc.h>

        if (!buffer) {
            LogWarn("Invalid Buffer Type requested");
            return false;
        }

        pyBufferMap_[bufferName] = buffer;
        pyBufferOwnershipMap_[bufferName] = true;
        buffer->setSize(bufferSize);
        BufferRAM* bufferRAM = buffer->getEditableRepresentation<BufferRAM>();
        bufferRAM->initialize();
        if (bufferRAM->getSize()==bufferSize)
            return true;
        return true;
    }
    return false;
}

void PyProcessorBase::addExistingPyBuffer(std::string bufferName, Buffer* buffer) {
    if (pyBufferMap_.find(bufferName)!=pyBufferMap_.end())
        LogWarn("Replacing buffer with similar name")
    pyBufferMap_[bufferName] = buffer;
    pyBufferOwnershipMap_[bufferName] = false;
}

Buffer* PyProcessorBase::getAllocatedPyBuffer(std::string bufferName) {
    Buffer* buffer = 0;
    if (pyBufferMap_.find(bufferName)!=pyBufferMap_.end())
        buffer = pyBufferMap_[bufferName];    
    return buffer;
}

bool PyProcessorBase::isValidPyBuffer(std::string bufferName) {
    if (pyBufferMap_.find(bufferName)!=pyBufferMap_.end())
        return true;    
    return false;
}

std::string PyProcessorBase::getPyBufferType(std::string bufferName) {
    Buffer* buffer = getAllocatedPyBuffer(bufferName);
    if (buffer)
        return buffer->getEditableRepresentation<BufferRAM>()->getDataFormat()->getString();    
    return "";
}

void* PyProcessorBase::getPyBufferData(std::string bufferName) {
    void* bufferData=0;
    Buffer* buffer = 0;
    if (pyBufferMap_.find(bufferName)!=pyBufferMap_.end()) {
        buffer = pyBufferMap_[bufferName];   
        bufferData = buffer->getEditableRepresentation<BufferRAM>()->getData();
    }
    return bufferData;
}

void PyProcessorBase::deallocatePyBuffer(std::string bufferName) {
    std::map<std::string, Buffer*>::iterator it = pyBufferMap_.find(bufferName);
    std::map<std::string, bool>::iterator itb = pyBufferOwnershipMap_.find(bufferName);
    if (it!=pyBufferMap_.end()) {
        Buffer* buffer = it->second;
        if (pyBufferOwnershipMap_[bufferName]) {
            pyBufferMap_.erase(it);
            delete buffer;
            pyBufferOwnershipMap_.erase(itb);
        }
        else
            LogInfo("Processor does not own the buffer. Cannot delete buffer " + bufferName)
    }
}

void PyProcessorBase::freeAllBuffers() {
    std::map<std::string, Buffer*>::iterator it;
    std::vector<std::string> bufferNames;
    for (it=pyBufferMap_.begin(); it!=pyBufferMap_.end(); ++it)
        bufferNames.push_back(it->first);
    for (size_t i=0; i<bufferNames.size(); i++)
        deallocatePyBuffer(bufferNames[i]);
    pyBufferMap_.clear();
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//Layer management
bool PyProcessorBase::allocateLayer(std::string layerName, std::string layerType, ivec2 layerDim) {
    //LogWarn("Not implemented")
    //allocate and cache buffer
    if (pyLayerMap_.find(layerName)==pyLayerMap_.end()) {
        Layer* layer = 0;
        if (0) layer = 0;
#define DataFormatIdMacro(i) else if (layerType == Data##i::str()) layer = new Layer(layerDim, Data##i::get());
#include <inviwo/core/util/formatsdefinefunc.h>

        if (!layer) {
            LogWarn("Invalid Buffer Type requested");
            return false;
        }

        pyLayerMap_[layerName] = layer;
        pyLayerOwnershipMap_[layerName] = true;
        LayerRAM* layerRAM = layer->getEditableRepresentation<LayerRAM>();
        layerRAM->initialize();
        if (layerRAM->getDimension().x == layerDim.x && layerRAM->getDimension().y == layerDim.y)
            return true;
        return true;
    }
    return false;
}

void PyProcessorBase::addExistingLayer(std::string layerName, Layer* layer) {
    if (pyLayerMap_.find(layerName)!=pyLayerMap_.end())
        LogWarn("Replacing layer with similar name")
    pyLayerMap_[layerName] = layer;
    pyLayerOwnershipMap_[layerName] = false;
}

Layer* PyProcessorBase::getAllocatedLayer(std::string layerName) {
    Layer* layer = 0;
    if (pyLayerMap_.find(layerName)!=pyLayerMap_.end())
        layer = pyLayerMap_[layerName];    
    return layer;
}

bool PyProcessorBase::isValidLayer(std::string layerName) {
    if (pyLayerMap_.find(layerName)!=pyLayerMap_.end())
        return true;    
    return false;
}

std::string PyProcessorBase::getLayerType(std::string layerName) {
    Layer* layer = getAllocatedLayer(layerName);
    if (layer)
        return layer->getEditableRepresentation<LayerRAM>()->getDataFormat()->getString();
    return "";
}

void* PyProcessorBase::getLayerData(std::string layerName) {
    void* layerData=0;
    Layer* layer = 0;
    if (pyLayerMap_.find(layerName)!=pyLayerMap_.end()) {
        layer = pyLayerMap_[layerName];
        layerData = layer->getEditableRepresentation<LayerRAM>()->getData();
    }
    return layerData;
}

void PyProcessorBase::deallocateLayer(std::string layerName) {
    std::map<std::string, Layer*>::iterator it = pyLayerMap_.find(layerName);
    std::map<std::string, bool>::iterator itb = pyLayerOwnershipMap_.find(layerName);
    if (it!=pyLayerMap_.end()) {
        Layer* layer = it->second;
        if (pyLayerOwnershipMap_[layerName]) {
            pyLayerMap_.erase(it);
            delete layer;
            pyLayerOwnershipMap_.erase(itb);
        }
        else
            LogInfo("Processor does not own the layer. Cannot delete layer " + layerName)
    }
}

void PyProcessorBase::freeAllLayers() {
    std::map<std::string, Layer*>::iterator it;
    std::vector<std::string> layerNames;
    for (it=pyLayerMap_.begin(); it!=pyLayerMap_.end(); ++it)
        layerNames.push_back(it->first);
    for (size_t i=0; i<layerNames.size(); i++)
        deallocateLayer(layerNames[i]);
    pyLayerMap_.clear();
}

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//Volume management
bool PyProcessorBase::allocateVolume(std::string volumeName, std::string volumeType, uvec3 volumeDim) {
    //LogWarn("Not implemented")
    //allocate and cache buffer
    if (pyVolumeMap_.find(volumeName)==pyVolumeMap_.end()) {
        Volume* volume = 0;
        if (0) volume = 0;
#define DataFormatIdMacro(i) else if (volumeType == Data##i::str()) volume = new Volume(volumeDim, Data##i::get());
#include <inviwo/core/util/formatsdefinefunc.h>

        if (!volume) {
            LogWarn("Invalid Volume Type requested");
            return false;
        }

        pyVolumeMap_[volumeName] = volume;
        pyVolumeOwnershipMap_[volumeName] = true;
        VolumeRAM* volumeRAM = volume->getEditableRepresentation<VolumeRAM>();
        volumeRAM->initialize();
        if (volumeRAM->getDimension().x == volumeDim.x && volumeRAM->getDimension().y == volumeDim.y
            && volumeRAM->getDimension().z == volumeDim.z)
            return true;
        return true;
    }
    return false;
}

void PyProcessorBase::addExistingVolume(std::string volumeName, Volume* volume) {
    if (pyBufferMap_.find(volumeName)!=pyBufferMap_.end())
        LogWarn("Replacing volume with similar name")
        pyVolumeMap_[volumeName] = volume;
    pyVolumeOwnershipMap_[volumeName] = false;
}

Volume* PyProcessorBase::getAllocatedVolume(std::string volumeName) {
    Volume* volume = 0;
    if (pyVolumeMap_.find(volumeName)!=pyVolumeMap_.end())
        volume = pyVolumeMap_[volumeName];
    return volume;
}

bool PyProcessorBase::isValidVolume(std::string volumeName) {
    if (pyVolumeMap_.find(volumeName)!=pyVolumeMap_.end())
        return true;
    return false;
}

std::string PyProcessorBase::getVolumeType(std::string volumeName) {
    Volume* volume = getAllocatedVolume(volumeName);
    if (volume)
        return volume->getEditableRepresentation<VolumeRAM>()->getDataFormat()->getString();
    return "";
}

void* PyProcessorBase::getVolumeData(std::string volumeName) {
    void* volumeData=0;
    Volume* volume = 0;
    if (pyVolumeMap_.find(volumeName)!=pyVolumeMap_.end()) {
        volume = pyVolumeMap_[volumeName];
        volumeData = volume->getEditableRepresentation<VolumeRAM>()->getData();
    }
    return volumeData;
}

void PyProcessorBase::deallocateVolume(std::string volumeName) {
    std::map<std::string, Volume*>::iterator it = pyVolumeMap_.find(volumeName);
    std::map<std::string, bool>::iterator itb = pyVolumeOwnershipMap_.find(volumeName);
    if (it!=pyVolumeMap_.end()) {
        Volume* volume = it->second;
        if (pyVolumeOwnershipMap_[volumeName]) {
            pyVolumeMap_.erase(it);
            delete volume;
            pyVolumeOwnershipMap_.erase(itb);
        }
        else
            LogInfo("Processor does not own the volume. Cannot delete volume " + volumeName)
    }
}

void PyProcessorBase::freeAllVolumes() {
    std::map<std::string, Volume*>::iterator it;
    std::vector<std::string> volumeNames;
    for (it=pyVolumeMap_.begin(); it!=pyVolumeMap_.end(); ++it)
        volumeNames.push_back(it->first);
    for (size_t i=0; i<volumeNames.size(); i++)
        deallocateVolume(volumeNames[i]);
    pyVolumeMap_.clear();
}

///////////////////////////////////////////////////////////////////////////////


} // namespace
