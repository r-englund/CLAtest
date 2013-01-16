#include "volumecl.h"

namespace inviwo {

VolumeCL::VolumeCL()
    : volumeImage_(0), VolumeRepresentation(ivec3(128,128,128), "UINT8") //TODO: use actual value
{
    initialize();
}

VolumeCL::VolumeCL(ivec3 dimensions)
    : volumeImage_(0), VolumeRepresentation(dimensions, "UINT8")
{
    initialize();
}

VolumeCL::~VolumeCL() {}

void VolumeCL::initialize() {}

void VolumeCL::deinitialize() {}




} // namespace
