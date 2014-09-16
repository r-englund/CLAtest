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
 * Main file authors: Peter Steneteg
 *
 *********************************************************************************/

#include "textureutils.h"
#include "canvasgl.h"
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/glwrap/bufferobjectarray.h>

namespace inviwo {

namespace util {
void glActivateTarget(ImageOutport& outport) {
    Image* outImage = outport.getData();
    ImageGL* outImageGL = outImage->getEditableRepresentation<ImageGL>();
    outImageGL->activateBuffer();
}

void glDeactivateCurrentTarget() { FrameBufferObject::deactivateFBO(); }

void glClearCurrentTarget() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void glActivateAndClearTarget(ImageOutport& outport) {
    glActivateTarget(outport);
    glClearCurrentTarget();
}

void glActivateAndClearTargets(PortGroup& portGroup) {
    portGroup.activate();
    glClearCurrentTarget();
}

void glUpdateAndActivateTarget(ImageOutport& outport, ImageInport& inport) {
    Image* outImage = outport.getData();
    ImageGL* outImageGL = outImage->getEditableRepresentation<ImageGL>();
    outImageGL->updateFrom(inport.getData()->getRepresentation<ImageGL>());
    outImageGL->activateBuffer();
}

void glBindTextures(const Image* image, bool color, bool depth, bool picking, GLenum colorTexUnit,
                    GLenum depthTexUnit, GLenum pickingTexUnit) {
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

void glBindColorTexture(const ImageInport& inport, GLenum texUnit) {
    glBindTextures(inport.getData(), true, false, false, texUnit, 0, 0);
}

void glBindColorTexture(const ImageOutport& outport, GLenum texUnit) {
    glBindTextures(outport.getConstData(), true, false, false, texUnit, 0, 0);
}

void glBindDepthTexture(const ImageInport& inport, GLenum texUnit) {
    glBindTextures(inport.getData(), false, true, false, 0, texUnit, 0);
}

void glBindDepthTexture(const ImageOutport& outport, GLenum texUnit) {
    glBindTextures(outport.getConstData(), false, true, false, 0, texUnit, 0);
}

void glBindPickingTexture(const ImageInport& inport, GLenum texUnit) {
    glBindTextures(inport.getData(), false, false, true, 0, 0, texUnit);
}

void glBindPickingTexture(const ImageOutport& outport, GLenum texUnit) {
    glBindTextures(outport.getConstData(), false, false, true, 0, 0, texUnit);
}

void glBindTextures(const Image* image, GLenum colorTexUnit, GLenum depthTexUnit) {
    glBindTextures(image, true, true, false, colorTexUnit, depthTexUnit, 0);
}

void glBindTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit) {
    glBindTextures(inport.getData(), true, true, false, colorTexUnit, depthTexUnit, 0);
}

void glBindTextures(const ImageOutport& outport, GLenum colorTexUnit, GLenum depthTexUnit) {
    glBindTextures(outport.getConstData(), true, true, false, colorTexUnit, depthTexUnit, 0);
}

void glBindTextures(const Image* image, GLenum colorTexUnit, GLenum depthTexUnit,
                    GLenum pickingTexUnit) {
    glBindTextures(image, true, true, true, colorTexUnit, depthTexUnit, pickingTexUnit);
}

void glBindTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit,
                    GLenum pickingTexUnit) {
    glBindTextures(inport.getData(), true, true, true, colorTexUnit, depthTexUnit, pickingTexUnit);
}

void glBindTextures(const ImageOutport& outport, GLenum colorTexUnit, GLenum depthTexUnit,
                    GLenum pickingTexUnit) {
    glBindTextures(outport.getConstData(), true, true, true, colorTexUnit, depthTexUnit,
                   pickingTexUnit);
}

void glUnbindTextures(const Image* image, bool color, bool depth, bool picking) {
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

void glUnbindColorTexture(const ImageInport& inport) {
    glUnbindTextures(inport.getData(), true, false, false);
}

void glUnbindColorTexture(const ImageOutport& outport) {
    glUnbindTextures(outport.getConstData(), true, false, false);
}

void glUnbindDepthTexture(const ImageInport& inport) {
    glUnbindTextures(inport.getData(), false, true, false);
}

void glUnbindDepthTexture(const ImageOutport& outport) {
    glUnbindTextures(outport.getConstData(), false, true, false);
}

void glUnbindPickingTexture(const ImageInport& inport) {
    glUnbindTextures(inport.getData(), false, false, true);
}

void glUnbindPickingTexture(const ImageOutport& outport) {
    glUnbindTextures(outport.getConstData(), false, false, true);
}

void glUnbindTextures(const Image* image) { glUnbindTextures(image, true, true, true); }

void glUnbindTextures(const ImageInport& inport) {
    glUnbindTextures(inport.getData(), true, true, true);
}

void glUnbindTextures(const ImageOutport& outport) {
    glUnbindTextures(outport.getConstData(), true, true, true);
}

void glSetTextureParameters(const Image* image, Shader* shader, const std::string samplerID) {
    vec2 dimensions = vec2(image->getDimension());
    shader->setUniform(samplerID + ".dimensions_", dimensions);
    shader->setUniform(samplerID + ".dimensionsRCP_", vec2(1.0f) / dimensions);
}

void glSetTextureParameters(const ImageInport& inport, Shader* shader,
                            const std::string samplerID) {
    glSetTextureParameters(inport.getData(), shader, samplerID);
}

void glSetTextureParameters(const ImageOutport& outport, Shader* shader,
                            const std::string samplerID) {
    glSetTextureParameters(outport.getConstData(), shader, samplerID);
}

BufferObjectArray* glEnableImagePlaneRect() {
    BufferObjectArray* rectArray = new BufferObjectArray();
    CanvasGL::attachImagePlanRect(rectArray);
    glDepthFunc(GL_ALWAYS);
    rectArray->bind();
    return rectArray;
}

void glDisableImagePlaneRect(BufferObjectArray* rectArray) {
    rectArray->unbind();
    glDepthFunc(GL_LESS);
}

void glSingleDrawImagePlaneRect() {
    BufferObjectArray* rectArray = glEnableImagePlaneRect();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableImagePlaneRect(rectArray);
    delete rectArray;
}

void glMultiDrawImagePlaneRect(int instances) {
    BufferObjectArray* rectArray = glEnableImagePlaneRect();
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, instances);
    glDisableImagePlaneRect(rectArray);
    delete rectArray;
}
}

}  // namespace
