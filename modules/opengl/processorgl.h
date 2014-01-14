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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

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
#include <modules/opengl/canvasgl.h>

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

    static inline void renderImagePlaneRect() {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_ALWAYS);
        CanvasGL::renderImagePlaneRect();
        glDepthFunc(GL_LESS);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

    static inline void renderImagePlaneRectInstanced(int instances) {
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_ALWAYS);
        CanvasGL::renderImagePlaneRectInstanced(instances);
        glDepthFunc(GL_LESS);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

    // deprecated
    static inline void renderQuad() {
        ivwDeprecatedMethod("renderImagePlaneRect()");
        renderImagePlaneRect();
    }
};

} // namespace

#endif // IVW_PROCESSORGL_H
