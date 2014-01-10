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
 * Primary author : Viktor Axelsson
 *
 **********************************************************************/

#include "findedges.h"
#include <inviwo/core/datastructures/image/imageram.h>

namespace inviwo {

ProcessorClassName(FindEdges, "FindEdges"); 
ProcessorCategory(FindEdges, "Image Operation");
ProcessorCodeState(FindEdges, CODE_STATE_EXPERIMENTAL);

FindEdges::FindEdges()
    : ProcessorGL(),
      inport_("inport"),
      outport_("outport", &inport_, COLOR_ONLY),
      alpha_("alpha", "Alpha", 0.5f, 0.0f, 1.0f)
{
    addPort(inport_);
    addPort(outport_);

    addProperty(alpha_);
}

FindEdges::~FindEdges() {}

void FindEdges::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("img_findedges.frag");
}

void FindEdges::deinitialize() {
    delete shader_;
    Processor::deinitialize();
}

void FindEdges::process() {    
    activateTarget(outport_);
    bindColorTexture(inport_, GL_TEXTURE0);

    shader_->activate();
    shader_->setUniform("inport_", 0);
    shader_->setUniform("alpha_", alpha_.get());
    shader_->setUniform("dimension_", vec2(1.f / outport_.getDimension()[0], 1.f / outport_.getDimension()[1]));
    renderImagePlaneRect();
    shader_->deactivate();

    deactivateCurrentTarget();
}

} // namespace
