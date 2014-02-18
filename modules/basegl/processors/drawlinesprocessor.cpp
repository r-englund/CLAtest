/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

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

void DrawLinesProcessor::addPoint(vec2 p) {
    lines_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->add(p);
}

void DrawLinesProcessor::clearPoints() {
    lines_->getAttributes(0)->getEditableRepresentation<Position2dBufferRAM>()->clear();
}

} // namespace
