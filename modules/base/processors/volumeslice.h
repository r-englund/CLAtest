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

#ifndef IVW_VOLUMESLICE_H
#define IVW_VOLUMESLICE_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <inviwo/core/datastructures/volume/volumeramslice.h>

namespace inviwo {

class IVW_MODULE_BASE_API VolumeSlice : public Processor {
public:
    VolumeSlice();
    ~VolumeSlice();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    VolumeInport inport_;
    ImageOutport outport_;

    IntOptionProperty coordinatePlane_;
    IntProperty sliceNumber_;
};

}

#endif //IVW_VOLUMESLICE_H
