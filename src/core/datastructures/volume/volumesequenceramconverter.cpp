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

#include <inviwo/core/datastructures/volume/volumesequenceramconverter.h>

namespace inviwo {

//////////VolumeSequenceDisk2VolumeDiskConverter/////////////

VolumeSequenceDisk2VolumeDiskConverter::VolumeSequenceDisk2VolumeDiskConverter()
    : RepresentationConverterType<VolumeDisk>()
{}

VolumeSequenceDisk2VolumeDiskConverter::~VolumeSequenceDisk2VolumeDiskConverter() {}

DataRepresentation* VolumeSequenceDisk2VolumeDiskConverter::createFrom(const DataRepresentation* source) {
    return NULL;
}
void VolumeSequenceDisk2VolumeDiskConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
}

//////////VolumeSequenceDisk2RAMConverter/////////////

VolumeSequenceDisk2RAMConverter::VolumeSequenceDisk2RAMConverter()
: RepresentationConverterType<VolumeSequenceRAM>()
{}

VolumeSequenceDisk2RAMConverter::~VolumeSequenceDisk2RAMConverter() {}

DataRepresentation* VolumeSequenceDisk2RAMConverter::createFrom(const DataRepresentation* source) {
    return NULL;
}
void VolumeSequenceDisk2RAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
}

//////////VolumeSequenceRAM2VolumeRAMConverter/////////////

VolumeSequenceRAM2VolumeRAMConverter::VolumeSequenceRAM2VolumeRAMConverter()
: RepresentationConverterType<VolumeRAM>()
{}

VolumeSequenceRAM2VolumeRAMConverter::~VolumeSequenceRAM2VolumeRAMConverter() {}

DataRepresentation* VolumeSequenceRAM2VolumeRAMConverter::createFrom(const DataRepresentation* source) {
    return NULL;
}
void VolumeSequenceRAM2VolumeRAMConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
}

} // namespace
