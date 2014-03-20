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

#include "volumesubset.h"
#include <inviwo/core/datastructures/volume/volumeramsubset.h>

namespace inviwo {

ProcessorClassName(VolumeSubset, "VolumeSubset");
ProcessorCategory(VolumeSubset, "Volume Operation");
ProcessorCodeState(VolumeSubset, CODE_STATE_STABLE);

VolumeSubset::VolumeSubset()
    : Processor(),
      inport_("volume.inport"),
      outport_("volume.outport"),
      enabled_("enabled", "Enable Operation", true),
      rangeX_("rangeX", "X Slices", 0, 256, 0, 256, 1, 1),
      rangeY_("rangeY", "Y Slices", 0, 256, 0, 256, 1, 1),
      rangeZ_("rangeZ", "Z Slices", 0, 256, 0, 256, 1, 1)
{
    addPort(inport_);
    addPort(outport_);
    addProperty(enabled_);
    addProperty(rangeX_);
    addProperty(rangeY_);
    addProperty(rangeZ_);
    dims_ = uvec3(1,1,1);
}

VolumeSubset::~VolumeSubset() {}

void VolumeSubset::initialize() {
    Processor::initialize();
}

void VolumeSubset::deinitialize() {
    Processor::deinitialize();
}

void VolumeSubset::process() {
    if (dims_ != inport_.getData()->getDimension()) {
        dims_ = inport_.getData()->getDimension();
        rangeX_.setRangeMax(static_cast<int>(dims_.x));
        rangeY_.setRangeMax(static_cast<int>(dims_.y));
        rangeZ_.setRangeMax(static_cast<int>(dims_.z));
    }

    if (enabled_.get()) {
        const VolumeRAM* vol = inport_.getData()->getRepresentation<VolumeRAM>();
        uvec3 dim = uvec3(static_cast<unsigned int>(rangeX_.get().y), static_cast<unsigned int>(rangeY_.get().y),
                          static_cast<unsigned int>(rangeZ_.get().y));
        uvec3 offset = uvec3(static_cast<unsigned int>(rangeX_.get().x), static_cast<unsigned int>(rangeY_.get().x),
                             static_cast<unsigned int>(rangeZ_.get().x));
        dim -= offset;

        if (dim == dims_)
            outport_.setConstData(inport_.getData());
        else {
            Volume* volume = new Volume(VolumeRAMSubSet::apply(vol, dim, offset));
            // pass meta data on
            const MetaDataOwner *metaOwner = dynamic_cast<const MetaDataOwner *>(inport_.getData());
            if (metaOwner) {
                volume->MetaDataOwner::operator=(*metaOwner);
            }
            outport_.setData(volume);
        }
    }
    else
        outport_.setConstData(inport_.getData());
}

} // inviwo namespace
