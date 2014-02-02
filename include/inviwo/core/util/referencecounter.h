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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_REFERENCE_COUNTER_H
#define IVW_REFERENCE_COUNTER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/util/assertion.h>

namespace inviwo {

/** \class ReferenceCounter
*
* Reference counter for an object. Object should be removed when reference count equals zero.
*/
class ReferenceCounter {
public:
    ReferenceCounter(): referenceCount_(1) {};
    virtual ~ReferenceCounter() { ivwAssert(getRefCount() == 0, "Deleting object with reference count != 0"); }
    /**
     * Increase reference count of this object.
     * Object should not be removed unless reference count is zero.
     *
     * @return New reference count after increasing it.
     */
    int increaseRefCount() { return ++referenceCount_; }

    /**
     * Decrease reference count of this object.
     * Object should not be removed unless reference count is zero.
     *
     * @return New reference count after decreasing it.
     */
    int decreaseRefCount() { return --referenceCount_; }
    /**
     * Get reference count of this object.
     * Object should not be removed unless reference count is zero.
     *
     */
    int getRefCount() const { return referenceCount_; }
private:
    int referenceCount_;


};


}; // namespace inviwo

#endif // IVW_REFERENCE_COUNTER_H