#ifndef IVW_MESHCLIPPING_H
#define IVW_MESHCLIPPING_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/datastructures/geometry/simplemesh.h>
#include <inviwo/core/datastructures/geometry/plane.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_MODULE_BASE_API MeshClipping : public ProcessorGL {
public:
	MeshClipping();
	~MeshClipping();

	InviwoProcessorInfo();

	void initialize();
	void deinitialize();

protected:
	virtual void process();
  
    Geometry* clipGeometryAgainstPlaneRevised(const Geometry*, Plane);
	Geometry* clipGeometryAgainstPlane(const Geometry*, Plane);
	float degreeToRad(float);

private:
	GeometryInport inport_;
	GeometryOutport outport_;
	
	BoolProperty clippingEnabled_;
    FloatVec3Property planePoint_;
    FloatVec3Property planeNormal_;
    BoolProperty renderAsPoints_;
	
	static const float EPSILON;
};
} // namespace

#endif // IVW_MESHCLIPPING_H