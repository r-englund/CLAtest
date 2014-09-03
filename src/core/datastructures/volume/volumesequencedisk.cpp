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
 * Main file authors: Erik Sundén
 *
 *********************************************************************************/

#include <inviwo/core/datastructures/volume/volumesequencedisk.h>

namespace inviwo {

VolumeSequenceDisk::VolumeSequenceDisk(uvec3 dimensions, const DataFormatBase* format)
    : VolumeRepresentation(dimensions, format) {
}

VolumeSequenceDisk::VolumeSequenceDisk(std::string srcFile, uvec3 dimensions, const DataFormatBase* format)
    : VolumeRepresentation(dimensions, format) {
}

VolumeSequenceDisk::VolumeSequenceDisk(const VolumeSequenceDisk& rhs)
    : VolumeRepresentation(rhs) {
}

VolumeSequenceDisk& VolumeSequenceDisk::operator=(const VolumeSequenceDisk& that) {
    if (this != &that) {
        VolumeRepresentation::operator=(that);
    }

    return *this;
}

VolumeSequenceDisk* VolumeSequenceDisk::clone() const {
    return new VolumeSequenceDisk(*this);
}

VolumeSequenceDisk::~VolumeSequenceDisk() {}

void VolumeSequenceDisk::addVolume(VolumeDisk* vd){
    volumes_.push_back(vd);
}

VolumeDisk* VolumeSequenceDisk::getVolume(int idx) const{
    if(idx < volumes_.size())
        return volumes_[idx];

    return NULL;
}

} // namespace
