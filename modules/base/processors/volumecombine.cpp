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
 * Contact: Erik Sundén
 *
 *********************************************************************************/

#include "volumecombine.h"
#include <inviwo/core/datastructures/volume/volumeramsubset.h>
#include <glm/gtx/vector_angle.hpp>

namespace inviwo {

ProcessorClassIdentifier(VolumeCombine, "org.inviwo.VolumeCombine");
ProcessorDisplayName(VolumeCombine,  "Volume Combine");
ProcessorTags(VolumeCombine, Tags::None);
ProcessorCategory(VolumeCombine, "Volume Operation");
ProcessorCodeState(VolumeCombine, CODE_STATE_STABLE);

VolumeCombine::VolumeCombine() : Processor()
      , inport1_("volume.inport1")
      , inport2_("volume.inport2")
      , inport3_("volume.inport3")
      , inport4_("volume.inport4")
      , outport_("volume.outport")
      , enabled_("enabled", "Enable Operation", true)
      , passThroughSelector_("passThroughSelector", "Choose Pass-Through Inport", 0, 0, 0)
{
    addPort(inport1_);
    addPort(inport2_);
    addPort(inport3_);
    addPort(inport4_);
    addPort(outport_);
    addProperty(enabled_);
    addProperty(passThroughSelector_);

    inports_.push_back(&inport1_);
    inports_.push_back(&inport2_);
    inports_.push_back(&inport3_);
    inports_.push_back(&inport4_);
}

VolumeCombine::~VolumeCombine() {}

void VolumeCombine::initialize() {
    Processor::initialize();
}

void VolumeCombine::deinitialize() {
    Processor::deinitialize();
}

void VolumeCombine::process() {
    /*if (dims_ != inport_.getData()->getDimension()) {
        dims_ = inport_.getData()->getDimension();
    }

    if (enabled_.get()) {
        uvec3 newDim = uvec3(1, 1, 1);

        // Assuming data is centered around origo, thus calculating largest size with respect to offset
        for (int i = 0; i < inports_.size(); ++i){
            const Volume* vol = inports_[i]->getData();
            vol
        }




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
            volume->copyMetaDataFrom(*inport_.getData());
            volume->dataMap_ = inport_.getData()->dataMap_;

            if (adjustBasisAndOffset_.get()) {
                vec3 volOffset = inport_.getData()->getOffset() + vec3(offset) / vec3(dims_);
                mat3 volBasis = inport_.getData()->getBasis();

                vec3 aVec(volBasis[0]);
                vec3 bVec(volBasis[1]);
                vec3 cVec(volBasis[2]);

                float alpha = glm::angle(bVec, cVec);
                float beta = glm::angle(cVec, aVec);
                float gamma = glm::angle(aVec, bVec);

                vec3 volLength(glm::length(aVec), glm::length(bVec), glm::length(cVec));
                // adjust volLength
                volLength *= vec3(dim) / vec3(dims_);

                float a = volLength.x;
                float b = volLength.y;
                float c = volLength.z;
                
                float v = std::sqrt(1 - std::cos(alpha)*std::cos(alpha) - std::cos(beta)*std::cos(beta) - std::cos(gamma)*std::cos(gamma)
                    - 2 * std::cos(alpha)*std::cos(beta)*std::cos(gamma));
                mat4 newBasisAndOffset(
                    a, b*std::cos(gamma), c*std::cos(beta), volOffset[0],
                    0.0f, b*std::sin(gamma), c*(std::cos(alpha) - std::cos(beta)*std::cos(gamma)) / std::sin(gamma), volOffset[1],
                    0.0f, 0.0f, c*v / std::sin(gamma), volOffset[2],
                    0.0f, 0.0f, 0.0f, 1.0f
                    );
                volume->setModelMatrix(glm::transpose(newBasisAndOffset));
            }
            else {
                // copy basis and offset
                volume->setModelMatrix(inport_.getData()->getModelMatrix());
            }
            outport_.setData(volume);
        }
    }
    else {
        outport_.setConstData(inport1_.getData());
    }*/
}

} // inviwo namespace
