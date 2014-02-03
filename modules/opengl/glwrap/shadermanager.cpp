 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013 Inviwo Foundation
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
 * Main file authors: Erik Sundén, Timo Ropinski
 *
 *********************************************************************************/

#include "shadermanager.h"
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/vectoroperations.h>
#include <inviwo/core/util/settings/systemsettings.h>
#include <modules/opengl/openglmodule.h>
#include <modules/opengl/processorgl.h>

namespace inviwo {

ShaderManager::ShaderManager() : FileObserver() {
    InviwoApplication::getRef().registerFileObserver(this);
    openGLInfoRef_ = NULL;
}

void ShaderManager::registerShader(Shader* shader) {
    shaders_.push_back(shader);
    const Shader::ShaderObjectMap* shaderObjects = shader->getShaderObjects();
    for(Shader::ShaderObjectMap::const_iterator it = shaderObjects->begin(); it != shaderObjects->end(); it++) {
        startFileObservation(it->second->getAbsoluteFileName());
        std::vector<std::string> shaderIncludes = it->second->getIncludeFileNames();
        for (size_t i=0;i<shaderIncludes.size();i++)
            startFileObservation(shaderIncludes[i]);
    }
}

void ShaderManager::unregisterShader(Shader* shader) {
    shaders_.erase(std::remove(shaders_.begin(), shaders_.end(), shader), shaders_.end());

    const Shader::ShaderObjectMap* shaderObjects = shader->getShaderObjects();
    for(Shader::ShaderObjectMap::const_iterator it = shaderObjects->begin(); it != shaderObjects->end(); it++) {
        stopFileObservation(it->second->getAbsoluteFileName());
        std::vector<std::string> shaderIncludes = it->second->getIncludeFileNames();
        for (size_t i=0;i<shaderIncludes.size();i++)
            stopFileObservation(shaderIncludes[i]);
    }
}

void ShaderManager::fileChanged(std::string shaderFilename) {
    if (dynamic_cast<BoolProperty*>(InviwoApplication::getPtr()->getSettingsByType<SystemSettings>()->getPropertyByIdentifier("shaderReloading"))->get()) { 
        if (isObserved(shaderFilename)) {
            bool successfulReload = false;
            for (size_t i=0; i<shaders_.size(); i++) {
                bool relink = false;
                const Shader::ShaderObjectMap* shaderObjects = shaders_[i]->getShaderObjects();
                for(Shader::ShaderObjectMap::const_iterator it = shaderObjects->begin(); it != shaderObjects->end(); it++) {
                    std::vector<std::string> shaderIncludes = it->second->getIncludeFileNames();
                    if (it->second->getAbsoluteFileName() == shaderFilename ||
                        std::find(shaderIncludes.begin(), shaderIncludes.end(), shaderFilename) != shaderIncludes.end()) {
                            successfulReload = it->second->rebuild();
                            relink = true;
                    }
                }
                if (relink) shaders_[i]->link();
            }
            if (successfulReload) {
                LogInfo(shaderFilename + " successfuly reloaded");
                InviwoApplication::getRef().playSound(InviwoApplication::IVW_OK);
                std::vector<Processor*> processors = InviwoApplication::getRef().getProcessorNetwork()->getProcessors();
                for (size_t i=0;i<processors.size();i++)
                    if (dynamic_cast<ProcessorGL*>(processors[i]))
                        processors[i]->invalidate(PropertyOwner::INVALID_RESOURCES);
            } else InviwoApplication::getRef().playSound(InviwoApplication::IVW_ERROR);
        }
    }
}

std::string ShaderManager::getGlobalGLSLHeader() {
    if (!openGLInfoRef_){
        OpenGLModule* openGLModule = getTypeFromVector<OpenGLModule>(InviwoApplication::getRef().getModules());
        if (openGLModule)
            openGLInfoRef_ = getTypeFromVector<OpenGLCapabilities>(openGLModule->getCapabilities());
    }

    if (openGLInfoRef_){
        return openGLInfoRef_->getCurrentGlobalGLSLHeader();
    }

    return "";
}

std::string ShaderManager::getGlobalGLSLFragmentDefines() {
    if (!openGLInfoRef_){
        OpenGLModule* openGLModule = getTypeFromVector<OpenGLModule>(InviwoApplication::getRef().getModules());
        if (openGLModule)
            openGLInfoRef_ = getTypeFromVector<OpenGLCapabilities>(openGLModule->getCapabilities());
    }

    if (openGLInfoRef_){
        return openGLInfoRef_->getCurrentGlobalGLSLFragmentDefines();
    }

    return "";
}

} // namespace
