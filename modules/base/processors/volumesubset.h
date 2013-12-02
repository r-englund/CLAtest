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

#ifndef IVW_VOLUMESUBSET_H
#define IVW_VOLUMESUBSET_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_MODULE_BASE_API VolumeSubset : public Processor {
public:
    VolumeSubset();
    ~VolumeSubset();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    VolumeInport inport_;
    VolumeOutport outport_;

    BoolProperty enabled_;

    IntMinMaxProperty rangeX_;
    IntMinMaxProperty rangeY_;
    IntMinMaxProperty rangeZ_;

    uvec3 dims_;
};

}

#endif //IVW_VOLUMESUBSET_H
