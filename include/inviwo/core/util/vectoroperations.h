/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
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

#ifndef IVW_VECTOROPERATIONS_H
#define IVW_VECTOROPERATIONS_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <vector>

namespace inviwo {

template<class T, class U>
T* getTypeFromVector(std::vector<U> vec) {
    if (vec.size() > 0) {
        for (size_t i=0; i<vec.size(); i++) {
            T* item = dynamic_cast<T*>(vec[i]);

            if (item)
                return item;
        }
    }

    return NULL;
}

template<class T, class U>
bool hasTypeInVector(const std::vector<U> vec) {
    for (size_t i=0; i<vec.size(); i++) {
        T* item = dynamic_cast<T*>(vec[i]);

        if (item)
            return true;
    }

    return false;
}

} // namespace

#endif // IVW_VECTOROPERATIONS_H
