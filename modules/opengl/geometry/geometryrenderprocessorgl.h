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

#ifndef IVW_GEOMETRYRENDERPROCESSORGL_H
#define IVW_GEOMETRYRENDERPROCESSORGL_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/rendering/geometryrenderer.h>
#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/processorgl.h>
#include <vector>

namespace inviwo {

class IVW_MODULE_OPENGL_API GeometryRenderProcessorGL : public ProcessorGL {
public:
    GeometryRenderProcessorGL();

    InviwoProcessorInfo();

    void deinitialize();

protected:
    virtual void process();
    void centerViewOnGeometry();

    GeometryMultiInport inport_;
    ImageOutport outport_;

    CameraProperty camera_;
    ButtonProperty centerViewOnGeometry_;

    std::vector<GeometryRenderer*> renderers_;
};

} // namespace

#endif // IVW_GEOMETRYRENDERPROCESSORGL_H
