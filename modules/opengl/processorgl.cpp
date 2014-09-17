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
 * Main file authors: Erik Sundén, Timo Ropinski
 *
 *********************************************************************************/

#include "processorgl.h"
#include <modules/opengl/canvasgl.h>

namespace inviwo {

ProcessorGL::ProcessorGL()
    : Processor(), rectArray_(NULL) {
    LogError("ProcessorGL is depricated, do not use!. Will be removed.");
}
ProcessorGL::~ProcessorGL() {
    delete rectArray_;
}

void ProcessorGL::activateTarget(ImageOutport& outport) {
    Image* outImage = outport.getData();
    ImageGL* outImageGL = outImage->getEditableRepresentation<ImageGL>();
    outImageGL->activateBuffer();
}

void ProcessorGL::deactivateCurrentTarget() {
    FrameBufferObject::deactivateFBO();
}

void ProcessorGL::clearCurrentTarget() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ProcessorGL::activateAndClearTarget(ImageOutport& outport) {
    activateTarget(outport);
    clearCurrentTarget();
}

void ProcessorGL::activateAndClearTargets(PortGroup& portGroup) {
    portGroup.activate();
    clearCurrentTarget();
}

void ProcessorGL::updateAndActivateTarget(ImageOutport& outport, ImageInport& inport) {
    Image* outImage = outport.getData();
    ImageGL* outImageGL = outImage->getEditableRepresentation<ImageGL>();
    outImageGL->updateFrom(inport.getData()->getRepresentation<ImageGL>());
    outImageGL->activateBuffer();
}


void ProcessorGL::bindColorTexture(const ImageInport& inport, GLenum texUnit) {
    bindTextures(inport.getData(), true, false, false, texUnit, 0, 0);
}

void ProcessorGL::bindColorTexture(const ImageOutport& outport, GLenum texUnit) {
    bindTextures(outport.getConstData(), true, false, false, texUnit, 0, 0);
}

void ProcessorGL::bindDepthTexture(const ImageInport& inport, GLenum texUnit) {
    bindTextures(inport.getData(), false, true, false, 0, texUnit, 0);
}

void ProcessorGL::bindDepthTexture(const ImageOutport& outport, GLenum texUnit) {
    bindTextures(outport.getConstData(), false, true, false, 0, texUnit, 0);
}

void ProcessorGL::bindPickingTexture(const ImageInport& inport, GLenum texUnit) {
    bindTextures(inport.getData(), false, false, true, 0, 0, texUnit);
}

void ProcessorGL::bindPickingTexture(const ImageOutport& outport, GLenum texUnit) {
    bindTextures(outport.getConstData(), false, false, true, 0, 0, texUnit);
}

void ProcessorGL::bindTextures(const Image* image, GLenum colorTexUnit, GLenum depthTexUnit) {
    bindTextures(image, true, true, false, colorTexUnit, depthTexUnit, 0);
}

void ProcessorGL::bindTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit) {
    bindTextures(inport.getData(), true, true, false, colorTexUnit, depthTexUnit, 0);
}

void ProcessorGL::bindTextures(const ImageOutport& outport, GLenum colorTexUnit, GLenum depthTexUnit) {
    bindTextures(outport.getConstData(), true, true, false, colorTexUnit, depthTexUnit, 0);
}

void ProcessorGL::bindTextures(const Image* image, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit) {
    bindTextures(image, true, true, true, colorTexUnit, depthTexUnit, pickingTexUnit);
}

void ProcessorGL::bindTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit) {
    bindTextures(inport.getData(), true, true, true, colorTexUnit, depthTexUnit, pickingTexUnit);
}

void ProcessorGL::bindTextures(const ImageOutport& outport, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit) {
    bindTextures(outport.getConstData(), true, true, true, colorTexUnit, depthTexUnit, pickingTexUnit);
}

void ProcessorGL::bindTextures(const Image* image, bool color, bool depth, bool picking, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit) {
    const ImageGL* imageGL = image->getRepresentation<ImageGL>();
    if (color) {
        const LayerGL* layer = imageGL->getColorLayerGL();
        if (layer) {
            layer->bindTexture(colorTexUnit);
        }
    }
    if (depth) {
        const LayerGL* layer = imageGL->getDepthLayerGL();
        if (layer) {
            layer->bindTexture(depthTexUnit);
        }
    }
    if (picking) {
        const LayerGL* layer = imageGL->getPickingLayerGL();
        if (layer) {
            layer->bindTexture(pickingTexUnit);
        }
    }
}

void ProcessorGL::unbindColorTexture(const ImageInport& inport) {
    unbindTextures(inport.getData(), true, false, false);
}

void ProcessorGL::unbindColorTexture(const ImageOutport& outport) {
    unbindTextures(outport.getConstData(), true, false, false);
}

void ProcessorGL::unbindDepthTexture(const ImageInport& inport) {
    unbindTextures(inport.getData(), false, true, false);
}

void ProcessorGL::unbindDepthTexture(const ImageOutport& outport) {
    unbindTextures(outport.getConstData(), false, true, false);
}

void ProcessorGL::unbindPickingTexture(const ImageInport& inport) {
    unbindTextures(inport.getData(), false, false, true);
}

void ProcessorGL::unbindPickingTexture(const ImageOutport& outport) {
    unbindTextures(outport.getConstData(), false, false, true);
}

void ProcessorGL::unbindTextures(const Image* image) {
    unbindTextures(image, true, true, true);
}

void ProcessorGL::unbindTextures(const ImageInport& inport) {
    unbindTextures(inport.getData(), true, true, true);
}

void ProcessorGL::unbindTextures(const ImageOutport& outport) {
    unbindTextures(outport.getConstData(), true, true, true);
}

void ProcessorGL::unbindTextures(const Image* image, bool color, bool depth, bool picking) {
    const ImageGL* imageGL = image->getRepresentation<ImageGL>();
    if (color) {
        const LayerGL* layer = imageGL->getColorLayerGL();
        if (layer) {
            layer->unbindTexture();
        }
    }
    if (depth) {
        const LayerGL* layer = imageGL->getDepthLayerGL();
        if (layer) {
            layer->unbindTexture();
        }
    }
    if (picking) {
        const LayerGL* layer = imageGL->getPickingLayerGL();
        if (layer) {
            layer->unbindTexture();
        }
    }
}

void ProcessorGL::setTextureParameters(const Image* image, Shader* shader, const std::string samplerID) {
    vec2 dimensions = vec2(image->getDimension());
    shader->setUniform(samplerID + ".dimensions_", dimensions);
    shader->setUniform(samplerID + ".dimensionsRCP_", vec2(1.0f)/dimensions);
}

void ProcessorGL::setTextureParameters(const ImageInport& inport, Shader* shader, const std::string samplerID) {
    setTextureParameters(inport.getData(), shader, samplerID);
}

void ProcessorGL::setTextureParameters(const ImageOutport& outport, Shader* shader, const std::string samplerID) {
    setTextureParameters(outport.getConstData(), shader, samplerID);
}

void ProcessorGL::setGlobalShaderParameters(Shader* shader) {
    setGlobalShaderParameters(shader, getOutports());
}

void ProcessorGL::setGlobalShaderParameters(Shader* shader, const std::vector<Outport*>& outports) {
    vec2 screenDimensions = vec2(0.0f,0.0f);

    for (size_t i=0; i<outports.size(); i++) {
        ImageOutport* imageOutport = dynamic_cast<ImageOutport*>(outports[i]);

        if (imageOutport) {
            screenDimensions = imageOutport->getDimension();
            break;
        }
    }

    shader->setUniform("screenDim_", screenDimensions);
    shader->setUniform("screenDimRCP_", vec2(1.0f,1.0f)/screenDimensions);
}
void ProcessorGL::enableDrawImagePlaneRect() {
    delete rectArray_;
    rectArray_ = new BufferObjectArray();
    CanvasGL::attachImagePlanRect(rectArray_);
    glDepthFunc(GL_ALWAYS);
    rectArray_->bind();
}

void ProcessorGL::disableDrawImagePlaneRect() {
    rectArray_->unbind();
    glDepthFunc(GL_LESS);
}

void ProcessorGL::renderImagePlaneRect() {
    enableDrawImagePlaneRect();
    singleDrawImagePlaneRect();
    disableDrawImagePlaneRect();
}

void ProcessorGL::renderImagePlaneRect(int instances) {
    enableDrawImagePlaneRect();
    multiDrawImagePlaneRect(instances);
    disableDrawImagePlaneRect();
}

// deprecated
void ProcessorGL::renderQuad() {
    ivwDeprecatedMethod("renderImagePlaneRect()");
    renderImagePlaneRect();
}

} // namespace
