/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

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
    coordinatePlane_.setCurrentStateAsDefault();
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

void VolumeSlice::process() {
    uvec3 dims = inport_.getData()->getDimension();

    switch (coordinatePlane_.get())
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
