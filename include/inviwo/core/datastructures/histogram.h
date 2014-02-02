/**********************************************************************
 * Copyright (C) 2014 Scientific Visualization Group - Linköping University
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

#ifndef IVW_HISTOGRAM_H
#define IVW_HISTOGRAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

//The NormalizedHistogram has a array of bins and all bins are normalized.
//It can be denormalized using the maxiumBinValue_.
class IVW_CORE_API NormalizedHistogram {

public:
    NormalizedHistogram(size_t);
    NormalizedHistogram(const NormalizedHistogram*);
    virtual ~NormalizedHistogram();

    std::vector<float>* getData();
    const std::vector<float>* getData() const;

    bool exists() const;

    void setValid(bool);
    bool isValid() const;

    void resize(size_t);

    void performNormalization();
    float getMaximumBinValue() const;

protected:
    std::vector<float>* data_;
    float maximumBinValue_; //The maximum count (used for normalization)
    bool valid_;
};

} // namespace

#endif // IVW_HISTOGRAM_H
