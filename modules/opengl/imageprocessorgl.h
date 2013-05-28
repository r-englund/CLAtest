#ifndef IVW_IMAGEPROCESSORGL_H
#define IVW_IMAGEPROCESSORGL_H

#include <modules/opengl/openglmoduledefine.h>
#include "inviwoopengl.h"
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl//processorgl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API ImageProcessorGL : public ProcessorGL {

public:
    ImageProcessorGL();
    ImageProcessorGL(std::string programFileName);
    virtual ~ImageProcessorGL();

    void initialize();
    void deinitialize();

protected:
    std::string programFileName_;
    Shader* program_;

    vec2 computeDepthRange(ImageInport* inport);
};

} // namespace

#endif // IVW_IMAGEPROCESSORGL_H
