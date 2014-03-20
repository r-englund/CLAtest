/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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

#include <inviwo/core/datastructures/histogram.h>
#include <algorithm>
#include <functional>

namespace inviwo {

NormalizedHistogram::NormalizedHistogram(size_t numberOfBins)
    : maximumBinValue_(1.f)
    , valid_(false) {
    data_ = new std::vector<float>(numberOfBins, 0.f);
}

NormalizedHistogram::NormalizedHistogram(const NormalizedHistogram* rhs) {
    if (rhs) {
        maximumBinValue_ = rhs->getMaximumBinValue();
        valid_ = rhs->isValid();
        data_ = new std::vector<float>(rhs->getData()->size(), 0.f);
        std::copy(rhs->getData()->begin(), rhs->getData()->end(), data_->begin());
    }
    else {
        data_ = new std::vector<float>(256, 0.f);
        maximumBinValue_ = 1.f;
        valid_ = false;
    }
}

NormalizedHistogram::NormalizedHistogram(const NormalizedHistogram &rhs) {
    maximumBinValue_ = rhs.getMaximumBinValue();
    valid_ = rhs.isValid();
    data_ = new std::vector<float>(rhs.getData()->size(), 0.f);
    std::copy(rhs.getData()->begin(), rhs.getData()->end(), data_->begin());
}

NormalizedHistogram::~NormalizedHistogram() {
    delete data_;
    data_ = NULL;
}

NormalizedHistogram& NormalizedHistogram::operator=(const NormalizedHistogram &rhs) {
    if (this != &rhs) {
        maximumBinValue_ = rhs.getMaximumBinValue();
        valid_ = rhs.isValid();
        if (!data_) {
            data_ = new std::vector<float>(rhs.getData()->size(), 0.f);
        }
        else {
            data_->resize(rhs.getData()->size());
        }
        std::copy(rhs.getData()->begin(), rhs.getData()->end(), data_->begin());
    }
    return *this;
}


std::vector<float>* NormalizedHistogram::getData() {
    return data_;
}

const std::vector<float>* NormalizedHistogram::getData() const {
    return data_;
}

bool NormalizedHistogram::exists() const {
    if (!data_ || data_->empty())
        return true;

    return false;
}

void NormalizedHistogram::setValid(bool valid) {
    valid_ = valid;
}

bool NormalizedHistogram::isValid() const {
    return valid_;
}

void NormalizedHistogram::resize(size_t numberOfBins) {
    if (numberOfBins != data_->size())
        data_->resize(numberOfBins);
}

void NormalizedHistogram::performNormalization() {
    //Find bin with largest count
    maximumBinValue_ = *std::max_element(data_->begin(), data_->end());

    //Normalize all bins with the largest count
    std::transform(data_->begin(), data_->end(), data_->begin(), std::bind2nd(std::multiplies<float>(), 1.0f / maximumBinValue_));
}

float NormalizedHistogram::getMaximumBinValue() const {
    return maximumBinValue_;
}

} // namespace

