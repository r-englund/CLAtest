#ifndef IVW_COMPOSITEPROCESSORGL_H
#define IVW_COMPOSITEPROCESSORGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/processorgl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API CompositeProcessorGL : public ProcessorGL {

public:
    CompositeProcessorGL();
    CompositeProcessorGL(std::string programFileName);

    void initialize();
    void deinitialize();

    // overloaded method called when INVALID_RESOURCE property is set
    virtual void initializeResources();

    void compositePortsToOutport(ImageOutport& outport, ImageInport& inport);

protected:
    std::string programFileName_;
    Shader* program_;
};

} // namespace

#endif // IVW_COMPOSITEPROCESSORGL_H
