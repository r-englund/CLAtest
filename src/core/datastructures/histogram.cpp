/**********************************************************************
 * Copyright (C) 2014 Scientific Visualization Group - Link�ping University
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

#include <inviwo/core/datastructures/histogram.h>

namespace inviwo {

NormalizedHistogram::NormalizedHistogram(size_t numberOfBins)
    : maximumBinValue_(1.f)
    , valid_(false) {
    data_ = new std::vector<float>(numberOfBins, 0.f);
}

NormalizedHistogram::NormalizedHistogram(const NormalizedHistogram* rhs)
    : maximumBinValue_(rhs->getMaximumBinValue())
    , valid_(rhs->isValid()) {
    data_ = new std::vector<float>(rhs->getData()->size(), 0.f);
    std::copy(rhs->getData()->begin(), rhs->getData()->end(), std::back_inserter(*data_));
}

NormalizedHistogram::~NormalizedHistogram() {
    delete data_;
    data_ = NULL;
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
    maximumBinValue_ = 0.f;

    for (std::vector<float>::const_iterator it = data_->begin()+1 ; it != data_->end(); ++it) {
        if ((*it)>maximumBinValue_)
            maximumBinValue_ = (*it);
    }

    //Normalize all bins with the largest count
    for (std::vector<float>::iterator it = data_->begin() ; it != data_->end(); ++it)
        (*it) /= maximumBinValue_;

    data_->at(0) = 1.0f;
}

float NormalizedHistogram::getMaximumBinValue() const {
    return maximumBinValue_;
}

} // namespace

