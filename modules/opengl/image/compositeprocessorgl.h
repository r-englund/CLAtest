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

#ifndef IVW_COMPOSITEPROCESSORGL_H
#define IVW_COMPOSITEPROCESSORGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/processorgl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API CompositeProcessorGL : public ProcessorGL {

public:
    CompositeProcessorGL();
    CompositeProcessorGL(std::string programFileName);

    void initialize();
    void deinitialize();

    // overloaded method called when INVALID_RESOURCE property is set
    virtual void initializeResources();

    void compositePortsToOutport(ImageOutport& outport, ImageInport& inport);

protected:
    std::string programFileName_;
    Shader* program_;
};

} // namespace

#endif // IVW_COMPOSITEPROCESSORGL_H
