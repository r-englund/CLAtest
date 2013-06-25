#ifndef IVW_GEOMETRYRENDERPROCESSORGL_H
#define IVW_GEOMETRYRENDERPROCESSORGL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/processorgl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API GeometryRenderProcessorGL : public ProcessorGL {
public:
    GeometryRenderProcessorGL();
    
    InviwoProcessorInfo();

protected:
    virtual void process();

    GeometryInport inport_;
    ImageOutport outport_;

    CameraProperty camera_;
};

} // namespace

#endif // IVW_GEOMETRYRENDERPROCESSORGL_H
