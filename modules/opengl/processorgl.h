#ifndef IVW_PROCESSORGL_H
#define IVW_PROCESSORGL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/processors/processor.h>
#include <modules/opengl/portgroup.h>
#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/image/imagegl.h>
#include <modules/opengl/canvasgl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API ProcessorGL : public Processor {

public:
    ProcessorGL();
    virtual ~ProcessorGL();

    void activateTarget(ImageOutport& outport);
    void deactivateCurrentTarget();

    void activateAndClearTarget(ImageOutport& outport);
    void activateAndClearTargets(PortGroup& portGroup);
    void clearCurrentTarget();

    void updateAndActivateTarget(ImageOutport& outport, ImageInport& inport);

    void bindColorTexture(const ImageInport& inport, GLenum texUnit);
    void bindColorTexture(const ImageOutport& outport, GLenum texUnit);
    void unbindColorTexture(const ImageInport& inport);
    void unbindColorTexture(const ImageOutport& outport);

    void bindDepthTexture(const ImageInport& inport, GLenum texUnit);
    void bindDepthTexture(const ImageOutport& outport, GLenum texUnit);
    void unbindDepthTexture(const ImageInport& inport);
    void unbindDepthTexture(const ImageOutport& outport);

    void bindPickingTexture(const ImageInport& inport, GLenum texUnit);
    void bindPickingTexture(const ImageOutport& outport, GLenum texUnit);
    void unbindPickingTexture(const ImageInport& inport);
    void unbindPickingTexture(const ImageOutport& outport);

    void bindTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit);
    void bindTextures(const ImageOutport& outport, GLenum colorTexUnit, GLenum depthTexUnit);
    void bindTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit);
    void bindTextures(const ImageOutport& outport, GLenum colorTexUnit, GLenum depthTexUnit, GLenum pickingTexUnit);
    void unbindTextures(const ImageInport& inport);
    void unbindTextures(const ImageOutport& outport);

    void setTextureParameters(const ImageInport& inport, Shader* shader, const std::string samplerID);
    void setTextureParameters(const ImageOutport& outport, Shader* shader, const std::string samplerID);

    void setGlobalShaderParameters(Shader* shader);

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

    // deprecated
    static inline void renderQuad() {
        ivwDeprecatedMethod("renderImagePlaneRect()");
        renderImagePlaneRect();
    }
};

} // namespace

#endif // IVW_PROCESSORGL_H
