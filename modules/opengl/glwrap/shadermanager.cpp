/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

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
