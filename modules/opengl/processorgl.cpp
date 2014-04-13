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

namespace inviwo {

ProcessorGL::ProcessorGL()
    : Processor(), rectArray_(NULL)
{}
ProcessorGL::~ProcessorGL() {
    delete rectArray_;
}

void ProcessorGL::activateTarget(ImageOutport& outport) {
    Image* outImage = outport.getData();
    ImageGL* outImageGL = outImage->getEditableRepresentation<ImageGL>();
    outImageGL->activateBuffer();
}

void ProcessorGL::deactivateCurrentTarget() {
    FrameBufferObject::deactivate();
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
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->getColorLayerGL()->bindTexture(texUnit);
}

void ProcessorGL::bindColorTexture(const ImageOutport& outport, GLenum texUnit) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->getColorLayerGL()->bindTexture(texUnit);
}

void ProcessorGL::bindDepthTexture(const ImageInport& inport, GLenum texUnit) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->getDepthLayerGL()->bindTexture(texUnit);
}

void ProcessorGL::bindDepthTexture(const ImageOutport& outport, GLenum texUnit) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->getDepthLayerGL()->bindTexture(texUnit);
}

void ProcessorGL::bindPickingTexture(const ImageInport& inport, GLenum texUnit) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->getPickingLayerGL()->bindTexture(texUnit);
}

void ProcessorGL::bindPickingTexture(const ImageOutport& outport, GLenum texUnit) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->getPickingLayerGL()->bindTexture(texUnit);
}

void ProcessorGL::bindTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->getColorLayerGL()->bindTexture(colorTexUnit);
    inImageGL->getDepthLayerGL()->bindTexture(depthTexUnit);
}

void ProcessorGL::bindTextures(const ImageOutport& outport, GLenum colorTexUnit, GLenum depthTexUnit) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->getColorLayerGL()->bindTexture(colorTexUnit);
    outImageGL->getDepthLayerGL()->bindTexture(depthTexUnit);
}

void ProcessorGL::bindTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->getColorLayerGL()->bindTexture(colorTexUnit);
    inImageGL->getDepthLayerGL()->bindTexture(depthTexUnit);
    inImageGL->getPickingLayerGL()->bindTexture(pickingTexUnit);
}

void ProcessorGL::bindTextures(const ImageOutport& outport, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->getColorLayerGL()->bindTexture(colorTexUnit);
    outImageGL->getDepthLayerGL()->bindTexture(depthTexUnit);
    outImageGL->getPickingLayerGL()->bindTexture(pickingTexUnit);
}

void ProcessorGL::unbindColorTexture(const ImageInport& inport) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->getColorLayerGL()->unbindTexture();
}

void ProcessorGL::unbindColorTexture(const ImageOutport& outport) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->getColorLayerGL()->unbindTexture();
}

void ProcessorGL::unbindDepthTexture(const ImageInport& inport) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->getDepthLayerGL()->unbindTexture();
}

void ProcessorGL::unbindDepthTexture(const ImageOutport& outport) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->getDepthLayerGL()->unbindTexture();
}

void ProcessorGL::unbindPickingTexture(const ImageInport& inport) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->getPickingLayerGL()->unbindTexture();
}

void ProcessorGL::unbindPickingTexture(const ImageOutport& outport) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->getPickingLayerGL()->unbindTexture();
}

void ProcessorGL::unbindTextures(const ImageInport& inport) {
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->getColorLayerGL()->unbindTexture();
    inImageGL->getDepthLayerGL()->unbindTexture();
    inImageGL->getPickingLayerGL()->unbindTexture();
}

void ProcessorGL::unbindTextures(const ImageOutport& outport) {
    const Image* outImage = outport.getConstData();
    const ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
    outImageGL->getColorLayerGL()->unbindTexture();
    outImageGL->getDepthLayerGL()->unbindTexture();
    outImageGL->getPickingLayerGL()->unbindTexture();
}

void ProcessorGL::setTextureParameters(const ImageInport& inport, Shader* shader, const std::string samplerID) {
    vec2 dimensions = vec2(inport.getDimension());
    shader->setUniform(samplerID + ".dimensions_", dimensions);
    shader->setUniform(samplerID + ".dimensionsRCP_", vec2(1.0f)/dimensions);
}

void ProcessorGL::setTextureParameters(const ImageOutport& outport, Shader* shader, const std::string samplerID) {
    vec2 dimensions = vec2(outport.getDimension());
    shader->setUniform(samplerID + ".dimensions_", dimensions);
    shader->setUniform(samplerID + ".dimensionsRCP_", vec2(1.0f)/dimensions);
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

void ProcessorGL::renderImagePlaneRect() {
    if(!rectArray_){
        rectArray_ = new BufferObjectArray();
        CanvasGL::attachImagePlanRect(rectArray_);
    }
    glDepthFunc(GL_ALWAYS);
    rectArray_->bind();
    CanvasGL::singleDrawImagePlaneRect();
    rectArray_->unbind();
    glDepthFunc(GL_LESS);
}

void ProcessorGL::renderImagePlaneRect(int instances) {
    if(!rectArray_){
        rectArray_ = new BufferObjectArray();
        CanvasGL::attachImagePlanRect(rectArray_);
    }
    glDepthFunc(GL_ALWAYS);
    rectArray_->bind();
    CanvasGL::multiDrawImagePlaneRect(instances);
    rectArray_->unbind();
    glDepthFunc(GL_LESS);
}

// deprecated
void ProcessorGL::renderQuad() {
    ivwDeprecatedMethod("renderImagePlaneRect()");
    renderImagePlaneRect();
}

} // namespace
