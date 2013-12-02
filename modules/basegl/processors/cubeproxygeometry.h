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

#ifndef IVW_CUBEPROXYGEOMETRY_H
#define IVW_CUBEPROXYGEOMETRY_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/vectorproperties.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API CubeProxyGeometry : public ProcessorGL {
public:
    CubeProxyGeometry();
    ~CubeProxyGeometry();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    VolumeInport inport_;
    GeometryOutport outport_;

    BoolProperty clippingEnabled_;

    IntMinMaxProperty clipX_;
    IntMinMaxProperty clipY_;
    IntMinMaxProperty clipZ_;

    uvec3 dims_;
    glm::mat4 basis_;
};

} // namespace

#endif // IVW_CUBEPROXYGEOMETRY_H
