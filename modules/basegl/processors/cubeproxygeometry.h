#ifndef IVW_CUBEPROXYGEOMETRY_H
#define IVW_CUBEPROXYGEOMETRY_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/vectorproperties.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API CubeProxyGeometry : public ProcessorGL {
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

    IntMinMaxProperty clipX_;
    IntMinMaxProperty clipY_;
    IntMinMaxProperty clipZ_;

    uvec3 dims_;
    glm::mat4 basis_;
};

} // namespace

#endif // IVW_CUBEPROXYGEOMETRY_H
