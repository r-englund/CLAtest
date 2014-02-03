 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file authors: Erik Sundén, Daniel Jönsson
 *
 *********************************************************************************/

#include <inviwo/core/datastructures/volume/volumeram.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>

namespace inviwo {

VolumeRAM::VolumeRAM(uvec3 dimensions, VolumeRepresentation::VolumeBorders border, const DataFormatBase* format)
    : VolumeRepresentation(dimensions, format, border), data_(NULL), histogram_(NULL), calculatingHistogram_(false)
{}

VolumeRAM::VolumeRAM(const VolumeRAM& rhs)
    : VolumeRepresentation(rhs), data_(NULL), histogram_(new NormalizedHistogram(rhs.histogram_)), calculatingHistogram_(false) {
}
VolumeRAM& VolumeRAM::operator=(const VolumeRAM& that) {
    if (this != &that) {
        VolumeRepresentation::operator=(that);
        histogram_ = new NormalizedHistogram(that.histogram_);
    }

    return *this;
}
VolumeRAM::~VolumeRAM() {
    deinitialize();
}

void VolumeRAM::initialize() {}

void VolumeRAM::deinitialize() {
    // Make sure that data is deinitialized in
    // child class (should not delete void pointer
    // since destructor will not be called for object.
    delete histogram_;
    histogram_ = NULL;
}

void* VolumeRAM::getData() {
    return data_;
}

const void* VolumeRAM::getData() const {
    return const_cast<void*>(data_);
}

bool VolumeRAM::hasNormalizedHistogram() const {
    return (histogram_ != NULL);
}

NormalizedHistogram* VolumeRAM::getNormalizedHistogram() {
    if (!calculatingHistogram_ && data_ && (!histogram_ || !histogram_->isValid()))
        calculateHistogram();

    return histogram_;
}

const NormalizedHistogram* VolumeRAM::getNormalizedHistogram() const {
    if (!calculatingHistogram_ && data_ && (!histogram_ || !histogram_->isValid()))
        calculateHistogram();

    return histogram_;
}

void VolumeRAM::calculateHistogram() const {
    calculatingHistogram_ = true;
    // TODO: using delta should be changeable from outside when requesting
    //       the histogram through getNormalizedHistogram()
    int maxDim = std::max(dimensions_.x, std::max(dimensions_.y, dimensions_.z));
    int delta = std::max(1, int(float(maxDim)/64.0f));
    histogram_ = VolumeRAMNormalizedHistogram::apply(this, histogram_, delta);
    calculatingHistogram_ = false;
}

VolumeRAM* createVolumeRAM(const uvec3& dimension, const DataFormatBase* format) {
    // TODO: Add more formats
    VolumeRAM* result = 0;

    switch (format->getId())
    {
        case NOT_SPECIALIZED:
            LogErrorCustom("createVolumeRAM", "Invalid format");
            break;
#define DataFormatIdMacro(i) case i: return new VolumeRAMCustomPrecision<Data##i::type, Data##i::bits>(dimension); break;
#include <inviwo/core/util/formatsdefinefunc.h>

        default:
            LogErrorCustom("createVolumeRAM", "Invalid format or not implemented");
            break;
    }

    return result;
}

} // namespace

