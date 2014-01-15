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

#include "drawlinesprocessor.h"

namespace inviwo {

ProcessorClassName(DrawLinesProcessor, "DrawLines"); 
ProcessorCategory(DrawLinesProcessor, "Line Rendering");
ProcessorCodeState(DrawLinesProcessor, CODE_STATE_EXPERIMENTAL); 

DrawLinesProcessor::DrawLinesProcessor()
    : CompositeProcessorGL(),
      inport_("inport"),
      outport_("outport", &inport_, COLOR_ONLY)
{
    addPort(inport_);
    addPort(outport_);
}

DrawLinesProcessor::~DrawLinesProcessor() {
}

void DrawLinesProcessor::initialize() {
    CompositeProcessorGL::initialize();

    lines_ = new Mesh(LINES, STRIP);
    lines_->initialize();
    lines_->addAttribute(new Position2dBuffer());

    addPoint(vec2(0.25f));
    addPoint(vec2(0.75f));

    lineRenderer_ = new MeshRenderer(lines_);
}

void DrawLinesProcessor::deinitialize() {
    CompositeProcessorGL::deinitialize();

    delete lineRenderer_;
    delete lines_;
}

void DrawLinesProcessor::process() {  
    activateAndClearTarget(outport_);

    lineRenderer_->render();

    deactivateCurrentTarget();

    compositePortsToOutport(outport_, inport_);
}

void DrawLinesProcessor::addPoint(vec2 p){
    lines_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->add(p);
}

void DrawLinesProcessor::clearPoints(){
    lines_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->clear();
}

} // namespace
