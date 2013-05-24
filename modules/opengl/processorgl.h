#ifndef IVW_PROCESSORGL_H
#define IVW_PROCESSORGL_H

#include <modules/opengl/openglmoduledefine.h>
#include "inviwoopengl.h"
#include "imagegl.h"
#include "glwrap/shader.h"
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API ProcessorGL : public Processor {

public:
    ProcessorGL();
    virtual ~ProcessorGL();

    void activateTarget(ImageOutport& outport);
    void deactivateCurrentTarget();

    void activateAndClearTarget(ImageOutport& outport);
    void clearCurrentTarget();

    void bindColorTexture(const ImageInport& inport, GLenum texUnit);
    void unbindColorTexture(const ImageInport& inport);
    void bindDepthTexture(const ImageInport& inport, GLenum texUnit);
    void unbindDepthTexture(const ImageInport& inport);
    void bindColorDepthTextures(const ImageInport& inport, GLenum colorTexUnit, GLenum depthTexUnit);
    void unbindColorDepthTextures(const ImageInport& inport);

    void setGlobalShaderParameters(Shader* shader);

    void renderImagePlaneQuad() const;
};

} // namespace

#endif // IVW_PROCESSORGL_H
