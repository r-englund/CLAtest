#ifndef IVW_CUBEPROXYGEOMETRY_H
#define IVW_CUBEPROXYGEOMETRY_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/vectorproperties.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>

namespace inviwo {

class IVW_MODULE_BASE_API CubeProxyGeometry : public ProcessorGL {
public:
    CubeProxyGeometry();
    ~CubeProxyGeometry();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    VolumeInport inport_;
    GeometryOutport outport_;

    BoolProperty clippingEnabled_;

    FloatMinMaxProperty test_;
    FloatVec2Property clipX_;
    FloatVec2Property clipY_;
    FloatVec2Property clipZ_;
};

} // namespace

#endif // IVW_CUBEPROXYGEOMETRY_H
