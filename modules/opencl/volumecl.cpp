#include <modules/opencl/volumecl.h>

namespace inviwo {

VolumeCL::VolumeCL()
    :  VolumeRepresentation(uvec3(128,128,128), "UINT8"), image3D_(0)
{
}

VolumeCL::VolumeCL(uvec3 dimensions)
    : image3D_(0), VolumeRepresentation(dimensions, "UINT8")
{
}

VolumeCL::~VolumeCL() {}







} // namespace
