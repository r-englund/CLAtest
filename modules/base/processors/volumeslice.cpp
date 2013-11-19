#include "volumeslice.h"

namespace inviwo {

ProcessorClassName(VolumeSlice, "VolumeSlice"); 
ProcessorCategory(VolumeSlice, "Volume Operation");
ProcessorCodeState(VolumeSlice, CODE_STATE_EXPERIMENTAL);

VolumeSlice::VolumeSlice()
    : Processor(),
    inport_("volume.inport"),
    outport_("image.outport", COLOR_ONLY),
    coordinatePlane_("coordinatePlane", "Coordinate Plane"),
    sliceNumber_("sliceNumber", "Slice Number", 1, 1, 256)
{
    addPort(inport_);
    addPort(outport_);

    coordinatePlane_.addOption("xy", "XY Plane", XY);
    coordinatePlane_.addOption("xz", "XZ Plane", XZ);
    coordinatePlane_.addOption("yz", "YZ Plane", YZ);
    coordinatePlane_.set(XY);
    addProperty(coordinatePlane_);
    addProperty(sliceNumber_);
}

VolumeSlice::~VolumeSlice() {}

void VolumeSlice::initialize() {
    Processor::initialize();
}

void VolumeSlice::deinitialize() {
    Processor::deinitialize();
}

void VolumeSlice::process(){
    uvec3 dims = inport_.getData()->getDimension();
    switch(coordinatePlane_.get())
    {
    case XY:
        sliceNumber_.setMaxValue(static_cast<int>(dims.z));
    	break;
    case XZ:
        sliceNumber_.setMaxValue(static_cast<int>(dims.y));
        break;
    case YZ:
        sliceNumber_.setMaxValue(static_cast<int>(dims.x));
        break;
    }

    const VolumeRAM* vol = inport_.getData()->getRepresentation<VolumeRAM>();
    ImageRAM* sliceImage = VolumeRAMSlice::apply(vol, coordinatePlane_.get(), static_cast<unsigned int>(sliceNumber_.get()-1));
    //sliceImage->resize(outport_.getData()->getDimension());
    outport_.setData(new Image(sliceImage));
    
}

} // inviwo namespace
