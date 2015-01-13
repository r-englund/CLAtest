/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.9
 *
 * Copyright (c) 2014-2015 Inviwo Foundation
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
 *********************************************************************************/

#ifndef IVW_PYPROCESSORBASE_H
#define IVW_PYPROCESSORBASE_H

#include <modules/pypackages/pypackagesmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/processors/processor.h>
#include <modules/python/pythonscript.h>
#include <inviwo/core/datastructures/buffer/buffer.h>
#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo {

class IVW_MODULE_PYPACKAGES_API PyProcessorBase : public Processor {
public:
    PyProcessorBase();
    virtual ~PyProcessorBase();

    virtual void initialize();
    virtual void deinitialize();
    virtual void process();

    //General    
    void runScript();
    void reloadScript();
    void onRunScriptButtonClicked(); 
    Buffer* convertLayerToBuffer(LayerRAM* layer);
    void freeAll();

    //Buffer management
    bool allocatePyBuffer(std::string bufferName, std::string bufferType, size_t bufferSize);    
    void addExistingPyBuffer(std::string bufferName, Buffer*);  
    Buffer* getAllocatedPyBuffer(std::string bufferName);
    bool isValidPyBuffer(std::string bufferName);
    std::string getPyBufferType(std::string bufferName);
    void* getPyBufferData(std::string bufferName);
    void deallocatePyBuffer(std::string bufferName);
    void freeAllBuffers();

    //Layer management
    bool allocateLayer(std::string layerName, std::string layerType, ivec2 layerDim);
    void addExistingLayer(std::string layerName, Layer*);
    Layer* getAllocatedLayer(std::string layerName);
    bool isValidLayer(std::string layerName);
    std::string getLayerType(std::string layerName);
    void* getLayerData(std::string layerName);
    void deallocateLayer(std::string layerName);
    void freeAllLayers();

    //Volume management
    bool allocateVolume(std::string volumeName, std::string volumeType, uvec3 volumeDim);
    void addExistingVolume(std::string volumeName, Volume*);
    Volume* getAllocatedVolume(std::string volumeName);
    bool isValidVolume(std::string volumeName);
    std::string getVolumeType(std::string volumeName);
    void* getVolumeData(std::string volumeName);
    void deallocateVolume(std::string volumeName);
    void freeAllVolumes();
    
protected:
    virtual void loadPythonScriptFile();
    FileProperty pythonScriptFile_;
    ButtonProperty runScript_;
    bool requiresRun_;
private:
    PythonScript script_;
    //Buffers
    std::map<std::string, Buffer*> pyBufferMap_;
    std::map<std::string, bool> pyBufferOwnershipMap_;
    //Layers
    std::map<std::string, Layer*> pyLayerMap_;
    std::map<std::string, bool> pyLayerOwnershipMap_;
    //Volumes
    std::map<std::string, Volume*> pyVolumeMap_;
    std::map<std::string, bool> pyVolumeOwnershipMap_;
};

} // namespace

#endif // IVW_PYPROCESSORBASE_H