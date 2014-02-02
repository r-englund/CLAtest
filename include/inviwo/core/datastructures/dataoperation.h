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

#ifndef IVW_DATAOPERATION_H
#define IVW_DATAOPERATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class DataRepresentation;

class IVW_CORE_API DataOperation {
public:
    DataOperation();
    virtual ~DataOperation();

    void setOutput(void* out);

    template<typename T>
    T* getOutput();

private:
    void* out_;
};

template<typename T>
inline T* DataOperation::getOutput() {
    return static_cast<T*>(out_);
}

} // namespace

#endif // IVW_DATAOPERATION_H
