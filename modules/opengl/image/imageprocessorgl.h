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

#ifndef IVW_IMAGEPROCESSORGL_H
#define IVW_IMAGEPROCESSORGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API ImageProcessorGL : public ProcessorGL {

public:
    ImageProcessorGL();
    ImageProcessorGL(std::string programFileName);

    void initialize();
    void deinitialize();

    // overloaded method called when INVALID_RESOURCE property is set
    virtual void initializeResources();

protected:
    std::string programFileName_;
    Shader* program_;

    vec2 computeDepthRange(ImageInport* inport);
};

} // namespace

#endif // IVW_IMAGEPROCESSORGL_H
