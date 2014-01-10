/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
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

#include "processorgl.h"

namespace inviwo {

ProcessorGL::ProcessorGL()
    : Processor()
{}
ProcessorGL::~ProcessorGL() {}

void ProcessorGL::activateTarget(ImageOutport& outport){
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

void ProcessorGL::updateAndActivateTarget(ImageOutport& outport, ImageInport& inport){
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

void ProcessorGL::bindTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit){
    const Image* inImage = inport.getData();
    const ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    inImageGL->getColorLayerGL()->bindTexture(colorTexUnit);
    inImageGL->getDepthLayerGL()->bindTexture(depthTexUnit);
    inImageGL->getPickingLayerGL()->bindTexture(pickingTexUnit);
}

void ProcessorGL::bindTextures(const ImageOutport& outport, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit){
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

} // namespace
