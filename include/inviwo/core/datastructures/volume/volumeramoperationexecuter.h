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

#ifndef IVW_VOLUMERAMOPERATIONEXECUTER_H
#define IVW_VOLUMERAMOPERATIONEXECUTER_H

#include <inviwo/core/datastructures/dataoperation.h>
#include <inviwo/core/datastructures/volume/volumeramslice.h>
#include <inviwo/core/datastructures/volume/volumeramsubset.h>

namespace inviwo {

//TODO: Make operation factory
template<typename T, size_t B>
void executeOperationOnVolumeRAMPrecision(DataOperation* dop){
    VolumeRAMSlice* volSliceDop = dynamic_cast<VolumeRAMSlice*>(dop);
    if (volSliceDop){
        volSliceDop->evaluate<T, B>();
        return;
    }
    VolumeRAMSubSet* volSubSetDop = dynamic_cast<VolumeRAMSubSet*>(dop);
    if (volSubSetDop){
        volSubSetDop->evaluate<T, B>();
        return;
    }
};

} // namespace

#endif // IVW_VOLUMERAMOPERATIONEXECUTER_H
