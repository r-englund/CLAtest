/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_CANVASPROCESSORGL_H
#define IVW_CANVASPROCESSORGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/util/canvas.h>
#include <inviwo/core/processors/canvasprocessor.h>

#include "processorgl.h"

namespace inviwo {

class IVW_MODULE_OPENGL_API CanvasProcessorGL : public CanvasProcessor {
public:
    CanvasProcessorGL();
    
    InviwoProcessorInfo();

    virtual void initialize();
    virtual void deinitialize();

    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel,
                            Property* modifiedProperty=0);

protected:
    virtual void process();

};

} // namespace

#endif // IVW_CANVASPROCESSORGL_H
