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

#ifndef IVW_LIGHTINGRAYCASTER_H
#define IVW_LIGHTINGRAYCASTER_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/volume/volumeraycastergl.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API LightingRaycaster : public VolumeRaycasterGL {
public:
    LightingRaycaster();
    
    InviwoProcessorInfo();

    void initializeResources();

protected:
    virtual void process();

private:
    VolumeInport volumePort_;
    ImageInport entryPort_;
    ImageInport exitPort_;
    VolumeInport lightVolumePort_;
    ImageOutport outport_;

    TransferFunctionProperty transferFunction_;
    BoolProperty enableLightColor_;
};

} // namespace

#endif // IVW_LIGHTINGRAYCASTER_H
