#ifndef IVW_MESHCLIPPING_H
#define IVW_MESHCLIPPING_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/geometryport.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <inviwo/core/datastructures/geometry/simplemeshram.h>
#include <inviwo/core/datastructures/geometry/plane.h>

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

	GeometryRAM* clipGeometry(const GeometryRAM*,SimpleMeshRAM*);
	GeometryRAM* clipGeometryAgainstPlane(const GeometryRAM*,Plane&);
	float degreeToRad(float);

private:
	GeometryInport inport_;
	GeometryOutport outport_;
	FloatProperty floatPropertyRotX_;
	FloatProperty floatPropertyRotY_;
	FloatProperty floatPropertyRotZ_;
	FloatProperty floatPropertyPlaneHeight;
	BoolProperty clippingEnabled_;
	Plane plane_;
	const float EPSILON;
};
} // namespace

#endif // IVW_MESHCLIPPING_H