/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#include "volumegl.h"
#include <inviwo/core/datastructures/volume/volume.h>
#include <modules/opengl/glwrap/shader.h>
#include <algorithm>

namespace inviwo {

VolumeGL::VolumeGL(uvec3 dimensions, const DataFormatBase* format, Texture3D* tex)
    : VolumeRepresentation(dimensions, format)
    , volumeTexture_(tex)
{
    initialize();
}

VolumeGL::VolumeGL(const VolumeGL& rhs)
    : VolumeRepresentation(rhs) {
    volumeTexture_ = rhs.volumeTexture_->clone();
}

VolumeGL& VolumeGL::operator=(const VolumeGL& rhs) {
    if (this != &rhs) {
        VolumeRepresentation::operator=(rhs);
        volumeTexture_ = rhs.volumeTexture_->clone();
    }

    return *this;
}

VolumeGL::~VolumeGL() {
    deinitialize();
}

VolumeGL* VolumeGL::clone() const {
    return new VolumeGL(*this);
}

void VolumeGL::initialize() {
    if (!volumeTexture_) {
        GLFormats::GLFormat glFormat = getGLFormats()->getGLFormat(getDataFormatId());
        volumeTexture_ = new Texture3D(dimensions_, glFormat, GL_LINEAR);
    }
}

void VolumeGL::deinitialize() {
    if (volumeTexture_ && volumeTexture_->decreaseRefCount() <= 0) {
        delete volumeTexture_;
        volumeTexture_ = NULL;
    }
}

void VolumeGL::bindTexture(GLenum texUnit) const {
    glActiveTexture(texUnit);
    volumeTexture_->bind();
}

void VolumeGL::unbindTexture() const {
    volumeTexture_->unbind();
}

void VolumeGL::setDimension(uvec3 dimensions) {
    dimensions_ = dimensions;
    volumeTexture_->uploadAndResize(NULL, dimensions_);
}

Texture3D* VolumeGL::getTexture() {
    return volumeTexture_;
}

const Texture3D* VolumeGL::getTexture() const {
    return volumeTexture_;
}

void VolumeGL::setVolumeUniforms(const Volume* volume, Shader* shader, const std::string& samplerID) const {
    vec3 dimF = vec3(dimensions_);
    shader->setUniform(samplerID + ".dimensions_", dimF);
    shader->setUniform(samplerID + ".dimensionsRCP_", vec3(1.f)/dimF);
    shader->setUniform(samplerID + ".volumeToWorldTransform_", volume->getWorldTransform());
    // adjust data scaling to volume data range
    if (volume->hasMetaData<Vec2MetaData>("DataRange")) {
        glm::vec2 dataRange;
        dataRange = volume->getMetaData<Vec2MetaData>("DataRange", dataRange);

        float datatypeMax = static_cast<float>(getDataFormat()->getMax());
        float datatypeMin = static_cast<float>(getDataFormat()->getMin());

        if ((std::abs(datatypeMin - dataRange.x) < glm::epsilon<float>())
            && (std::abs(datatypeMax - dataRange.y) < glm::epsilon<float>())) 
        {
            // no change, use original GL scaling factor if volume data range is equal to type data range
            shader->setUniform(samplerID + ".formatScaling_", getGLFormats()->getGLFormat(getDataFormatId()).scaling);
        }
        else {
            // TODO: consider lower bounds of data range as well (offset in shader before scaling)
            float scalingFactor = datatypeMax / dataRange.y;
            // offset scaling because of reversed scaling in the shader, i.e. (1 - formatScaling_)
            shader->setUniform(samplerID + ".formatScaling_", 1.0f - scalingFactor);
        }
    }
    else {
        // fall-back: no data range given for the volume
        shader->setUniform(samplerID + ".formatScaling_", getGLFormats()->getGLFormat(getDataFormatId()).scaling);
    }
}

} // namespace
