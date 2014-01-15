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

#ifndef IVW_DRAWLINESPROCESSOR_H
#define IVW_DRAWLINESPROCESSOR_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/image/compositeprocessorgl.h>
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <modules/opengl/rendering/meshglrenderer.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API DrawLinesProcessor : public CompositeProcessorGL {
public:
    DrawLinesProcessor();
    ~DrawLinesProcessor();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

    bool isReady() const { return true; }

protected:
    void addPoint(vec2);
    void clearPoints();

    virtual void process();

private:
    ImageInport inport_;
    ImageOutport outport_;

    Mesh* lines_;
    MeshGLRenderer* lineRenderer_;
};

} // namespace

#endif // IVW_DRAWLINESPROCESSOR_H
