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

#ifndef IVW_VOLUMEOPERATION_H
#define IVW_VOLUMEOPERATION_H

#include <inviwo/core/datastructures/dataoperation.h>

namespace inviwo {

class VolumeRepresentation;

class IVW_CORE_API VolumeOperation : public DataOperation {
public:
    VolumeOperation(const VolumeRepresentation* in) : DataOperation(), in_(in){}
    virtual ~VolumeOperation(){}

    const VolumeRepresentation* getInputVolume() { return in_; }

private:
    const VolumeRepresentation* in_;
};

} // namespace

#endif // IVW_VOLUMEOPERATION_H
