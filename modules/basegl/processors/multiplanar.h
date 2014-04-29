#ifndef IVW_MULTIPLANAR_H
#define IVW_MULTIPLANAR_H

#include <modules/basegl/baseglmoduledefine.h>
#include <modules/opengl/processorgl.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/transferfunctionproperty.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API MultiPlanar : public ProcessorGL { 
public:
    MultiPlanar();
    virtual ~MultiPlanar(){}

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

    Geometry* createSliceXYGeometry();
    Geometry* createSliceXZGeometry();
    Geometry* createSliceYZGeometry();

private:
    ImageInport sliceXYPort_;
    ImageInport sliceXZPort_;
    ImageInport sliceYZPort_;
    ImageOutport outport_;

    CameraProperty camera_;

    BoolProperty showSliceXY_;
    BoolProperty showSliceXZ_;
    BoolProperty showSliceYZ_;

    IntProperty sliceXYPos_;
    IntProperty sliceXZPos_;
    IntProperty sliceYZPos_;

    Shader* shader_;
};

} // namespace

#endif // IVW_MULTIPLANAR_H

