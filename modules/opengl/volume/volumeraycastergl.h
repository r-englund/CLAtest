/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file author: Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_VOLUMERAYCASTERGL_H
#define IVW_VOLUMERAYCASTERGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/transferfunction.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/baseoptionproperty.h>

#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API VolumeRaycasterGL : public ProcessorGL {

public:
    VolumeRaycasterGL();
    VolumeRaycasterGL(std::string shaderFileName);

    virtual void initialize();
    virtual void deinitialize();

    // overloaded method called when INVALID_RESOURCE property is set
    virtual void initializeResources();

    // convenience functions for texture/volume binding
    void bindTransferFunction(const TransferFunction& tf, GLenum tfTexUnit);
    void bindVolume(const VolumeInport& inport, GLenum volTexUnit);
    void setVolumeParameters(const VolumeInport& inport, Shader* shader, const std::string samplerID);
    void setGlobalShaderParameters(Shader* shader);

protected:

    /** 
     * Returns the compositing mode, which will replace
     * RC_APPLY_COMPOSITING(result, color, samplePos, voxel, gradient, t, tDepth, tIncr)
     * in the shader.
     */
    virtual std::string getCompositingDefine();
    Shader* shader_;
    std::string shaderFileName_;

    FloatProperty samplingRate_;
    FloatProperty isoValue_;

    OptionPropertyString classificationMode_;
    OptionPropertyString gradientComputationMode_;
    OptionPropertyString shadingMode_;
    OptionPropertyString compositingMode_;

    FloatVec3Property lightPosition_;
    FloatVec3Property lightColorAmbient_;
    FloatVec3Property lightColorDiffuse_;
    FloatVec3Property lightColorSpecular_;
    IntProperty lightSpecularExponent_;
    BoolProperty applyLightAttenuation_;
    FloatVec3Property lightAttenuation_;

    CameraProperty camera_;

    void addShadingProperties();

private:
    void addBasicProperties();
};

} // namespace

#endif // IVW_VOLUMERAYCASTERGL_H
