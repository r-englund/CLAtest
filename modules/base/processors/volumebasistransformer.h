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
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#ifndef IVW_VOLUMEBASISTRANSFORMER_H
#define IVW_VOLUMEBASISTRANSFORMER_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/volumeport.h>
#include <modules/base/basemoduledefine.h>

namespace inviwo {

class IVW_MODULE_BASE_API VolumeBasisTransformer : public Processor {

public:
    VolumeBasisTransformer();
    ~VolumeBasisTransformer();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    VolumeInport inport_;
    VolumeOutport outport_;

    FloatVec3Property lengths_;
    FloatVec3Property angels_;
    FloatVec3Property offset_;

    mat4 orgBasisAndOffset_;
};

}

#endif
