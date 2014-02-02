/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Link�ping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

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

