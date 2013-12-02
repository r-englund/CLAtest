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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_VOLUMEPROCESSORGL_H
#define IVW_VOLUMEPROCESSORGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl//processorgl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API VolumeProcessorGL : public ProcessorGL {

public:
    VolumeProcessorGL();
    virtual ~VolumeProcessorGL();

    void initialize();
    void deinitialize();
};

} // namespace

#endif // IVW_VOLUMEPROCESSORGL_H
