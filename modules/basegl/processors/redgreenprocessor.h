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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_REDGREENPROCESSOR_H
#define IVW_REDGREENPROCESSOR_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/volumeport.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/glwrap/shader.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API RedGreenProcessor : public ProcessorGL {
public:
    RedGreenProcessor();
    ~RedGreenProcessor();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    ImageOutport outportRed_;
    ImageOutport outportGreen_;
};

} // namespace

#endif // IVW_REDGREENPROCESSOR_H
