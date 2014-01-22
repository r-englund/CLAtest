/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

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

    //const VolumeRAM* vol = inport_.getData()->getRepresentation<VolumeRAM>();
    //LayerRAM* sliceImage = VolumeRAMSlice::apply(vol, coordinatePlane_.get(), static_cast<unsigned int>(sliceNumber_.get()-1));
    //sliceImage->resize(outport_.getData()->getDimension());
    //outport_.setData(new Image(sliceImage));
    
}

} // inviwo namespace
