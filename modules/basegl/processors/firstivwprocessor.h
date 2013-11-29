#ifndef IVW_FIRSTIVWPROCESSOR_H
#define IVW_FIRSTIVWPROCESSOR_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API FirstIvwProcessor : public ProcessorGL {
public:
    FirstIvwProcessor();
    
    InviwoProcessorInfo();

protected:
    virtual void process();

private:
    FloatVec3Property color_;
    ImageOutport outport_;
};

} // namespace

#endif // VRN_FIRSTIVWPROCESSOR_H
