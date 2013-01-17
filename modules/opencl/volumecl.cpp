#include "modules/opencl/volumecl.h"

namespace inviwo {

VolumeCL::VolumeCL()
    : image3D_(0), VolumeRepresentation(ivec3(128,128,128), "UINT8") //TODO: use actual value
{
}

VolumeCL::VolumeCL(ivec3 dimensions)
    : image3D_(0), VolumeRepresentation(dimensions, "UINT8")
{
}

VolumeCL::~VolumeCL() {}







} // namespace
