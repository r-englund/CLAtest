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
 * Main file authors: Erik Sundén, Daniel Jönsson
 *
 *********************************************************************************/

#ifndef IVW_PROCESSORGL_H
#define IVW_PROCESSORGL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/processors/processor.h>
#include <modules/opengl/portgroup.h>
#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>
#include <modules/opengl/image/imagegl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API ProcessorGL : public Processor {

public:
    ProcessorGL();
    virtual ~ProcessorGL();

    static void activateTarget(ImageOutport& outport);
    static void deactivateCurrentTarget();

    static void activateAndClearTarget(ImageOutport& outport);
    static void activateAndClearTargets(PortGroup& portGroup);
    static void clearCurrentTarget();

    static void updateAndActivateTarget(ImageOutport& outport, ImageInport& inport);

    static void bindColorTexture(const ImageInport& inport, GLenum texUnit);
    static void bindColorTexture(const ImageOutport& outport, GLenum texUnit);
    static void unbindColorTexture(const ImageInport& inport);
    static void unbindColorTexture(const ImageOutport& outport);

    static void bindDepthTexture(const ImageInport& inport, GLenum texUnit);
    static void bindDepthTexture(const ImageOutport& outport, GLenum texUnit);
    static void unbindDepthTexture(const ImageInport& inport);
    static void unbindDepthTexture(const ImageOutport& outport);

    static void bindPickingTexture(const ImageInport& inport, GLenum texUnit);
    static void bindPickingTexture(const ImageOutport& outport, GLenum texUnit);
    static void unbindPickingTexture(const ImageInport& inport);
    static void unbindPickingTexture(const ImageOutport& outport);

    static void bindTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit);
    static void bindTextures(const ImageOutport& outport, GLenum colorTexUnit, GLenum depthTexUnit);
    static void bindTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit);
    static void bindTextures(const ImageOutport& outport, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit);
    static void unbindTextures(const ImageInport& inport);
    static void unbindTextures(const ImageOutport& outport);

    static void setTextureParameters(const ImageInport& inport, Shader* shader, const std::string samplerID);
    static void setTextureParameters(const ImageOutport& outport, Shader* shader, const std::string samplerID);

    void setGlobalShaderParameters(Shader* shader);

    static void setGlobalShaderParameters(Shader* shader, const std::vector<Outport*>& outports);

    static inline void singleDrawImagePlaneRect() {
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    static inline void multiDrawImagePlaneRect(int instances) {
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, instances);
    }

    void enableDrawImagePlaneRect();
    void disableDrawImagePlaneRect();

    void renderImagePlaneRect();
    void renderImagePlaneRect(int instances);

    // deprecated
    void renderQuad();

private:
    static inline void bindTextures(const Image* image, bool color, bool depth, bool picking, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit);
    static inline void unbindTextures(const Image* image, bool color, bool depth, bool picking);

    BufferObjectArray* rectArray_;
};

} // namespace

#endif // IVW_PROCESSORGL_H
