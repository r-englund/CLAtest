#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>

namespace inviwo {

// Volume Inport
VolumeInport::VolumeInport(std::string identifier)
: DataInport<Volume>(identifier)
{}

VolumeInport::~VolumeInport() {
}

void VolumeInport::initialize() {}

void VolumeInport::deinitialize() {}


uvec3 VolumeInport::getColorCode() const { 
    return uvec3(188,101,101); 
}

bool VolumeInport::canConnectTo(Port* port) const {
	if (dynamic_cast<VolumeOutport*>(port))
		return true;
	else
		return false;
}

// Volume Outport
VolumeOutport::VolumeOutport(std::string identifier)
: DataOutport<Volume>(identifier)
{
    data_ = new StandardVolume();
}

VolumeOutport::~VolumeOutport() {}

void VolumeOutport::initialize() {}

void VolumeOutport::deinitialize() {}

uvec3 VolumeOutport::getColorCode() const { 
    return uvec3(188,101,101); 
}

bool VolumeOutport::canConnectTo(Port* port) const {
	if (dynamic_cast<VolumeInport*>(port))
		return true;
	else
		return false;
}

} // namespace