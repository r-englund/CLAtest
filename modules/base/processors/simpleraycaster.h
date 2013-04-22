#ifndef IVW_SIMPLERAYCASTER_H
#define IVW_SIMPLERAYCASTER_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/properties/properties.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/volumegl.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

class IVW_MODULE_BASE_API SimpleRaycaster : public ProcessorGL {
public:
    SimpleRaycaster();
    ~SimpleRaycaster();
    virtual Processor* create() const;

    void initialize();
    void deinitialize();

    virtual std::string getClassName() const { return "SimpleRaycaster"; }
    virtual std::string getCategory() const  { return "Ray Caster"; }
    virtual CodeState getCodeState() const   { return CODE_STATE_EXPERIMENTAL; }

protected:
    virtual void process();

private:
    VolumePort volumePort_;
    ImagePort entryPort_;
    ImagePort exitPort_;
    ImagePort outport_;

    BoolProperty enableShading_;
    FloatVec3Property lightSourcePos_;
	BoolProperty enableMIP_;
    FloatProperty samplingRate_;
    TransferFunctionProperty transferFunction_;

    Shader* shader_;
};

} // namespace

#endif // IVW_SIMPLERAYCASTER_H
