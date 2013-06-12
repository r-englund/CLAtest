#ifndef IVW_SIMPLERAYCASTER_H
#define IVW_SIMPLERAYCASTER_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/properties/properties.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/volume/volumeraycastergl.h>
#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

class IVW_MODULE_BASE_API SimpleRaycaster : public VolumeRaycasterGL {
public:
    SimpleRaycaster();
    
    InviwoProcessorInfo();

protected:
    virtual void process();

private:
    VolumeInport volumePort_;
    ImageInport entryPort_;
    ImageInport exitPort_;
    ImageOutport outport_;

    BoolProperty enableShading_;
    FloatVec3Property lightSourcePos_;
	BoolProperty enableMIP_;
    FloatProperty samplingRate_;
    TransferFunctionProperty transferFunction_;
};

} // namespace

#endif // IVW_SIMPLERAYCASTER_H
