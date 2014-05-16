/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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

#include <inviwo/core/util/canvas.h>
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>

namespace inviwo {

EventHandler* eventHandler_();

Geometry* Canvas::screenAlignedRect_ = NULL;

Canvas::Canvas(uvec2 dimensions)
    : shared_(true)
    , dimensions_(dimensions)
    , processorNetworkEvaluator_(0) {
    pickingContainer_ = new PickingContainer();

    if (!screenAlignedRect_) {
        shared_ = false;
        Position2dBuffer* verticesBuffer = new Position2dBuffer();
        Position2dBufferRAM* verticesBufferRAM = verticesBuffer->getEditableRepresentation<Position2dBufferRAM>();
        verticesBufferRAM->add(vec2(-1.0f, -1.0f));
        verticesBufferRAM->add(vec2(1.0f, -1.0f));
        verticesBufferRAM->add(vec2(-1.0f, 1.0f));
        verticesBufferRAM->add(vec2(1.0f, 1.0f));
        TexCoord2dBuffer* texCoordsBuffer = new TexCoord2dBuffer();
        TexCoord2dBufferRAM* texCoordsBufferRAM = texCoordsBuffer->getEditableRepresentation<TexCoord2dBufferRAM>();
        texCoordsBufferRAM->add(vec2(0.0f, 0.0f));
        texCoordsBufferRAM->add(vec2(1.0f, 0.0f));
        texCoordsBufferRAM->add(vec2(0.0f, 1.0f));
        texCoordsBufferRAM->add(vec2(1.0f, 1.0f));
        Mesh* screenAlignedRectMesh = new Mesh(TRIANGLES, STRIP);
        screenAlignedRectMesh->addAttribute(verticesBuffer);
        screenAlignedRectMesh->addAttribute(texCoordsBuffer);
        screenAlignedRect_ = screenAlignedRectMesh;
    }
}

Canvas::~Canvas() {
    if (!shared_)
        delete screenAlignedRect_;

    delete pickingContainer_;
}

void Canvas::initialize() {
    if (!pickingContainer_)
        pickingContainer_ = new PickingContainer();
}

void Canvas::deinitialize() {}

void Canvas::render(const Image* im, LayerType layerType/* = COLOR_LAYER*/) {}

void Canvas::activate() {}

void Canvas::resize(uvec2 size) {
    uvec2 previousDimensions = dimensions_;
    dimensions_ = size;

    if (getProcessorNetworkEvaluator()) {
        getProcessorNetworkEvaluator()->activateDefaultRenderContext();
        ResizeEvent* resizeEvent = new ResizeEvent(dimensions_);
        resizeEvent->setPreviousSize(previousDimensions);
        getProcessorNetworkEvaluator()->propagateResizeEvent(this, resizeEvent);
        delete resizeEvent;
    }
}

uvec2 Canvas::getDimension() {
    return dimensions_;
}

void Canvas::update() {}

void Canvas::setNetworkEvaluator(ProcessorNetworkEvaluator* networkEvaluator) {
    processorNetworkEvaluator_ = networkEvaluator;
}

ProcessorNetworkEvaluator* Canvas::getProcessorNetworkEvaluator() {
    return processorNetworkEvaluator_;
}

void Canvas::activateDefaultRenderContext(){
    getProcessorNetworkEvaluator()->activateDefaultRenderContext();
}

void Canvas::interactionEvent(InteractionEvent* e) {
    getProcessorNetworkEvaluator()->propagateInteractionEvent(this, e);
}

void Canvas::mousePressEvent(MouseEvent* e) {
    if (e->button() == MouseEvent::MOUSE_BUTTON_LEFT) {
        bool picked = pickingContainer_->performPick(mousePosToPixelCoordinates(e->pos()));

        if (!picked)
            interactionEvent(e);
    }
    else
        interactionEvent(e);
}

void Canvas::mouseReleaseEvent(MouseEvent* e) {
    pickingContainer_->setPickableSelected(false);
    interactionEvent(e);
}

void Canvas::mouseMoveEvent(MouseEvent* e) {
    if (pickingContainer_->isPickableSelected())
        pickingContainer_->movePicked(mousePosToPixelCoordinates(e->pos()));
    else
        interactionEvent(e);
}

void Canvas::mouseWheelEvent(MouseEvent* e) {
    interactionEvent(e);
}

void Canvas::keyPressEvent(KeyboardEvent* e) {
    interactionEvent(e);
}

void Canvas::keyReleaseEvent(KeyboardEvent* e) {
    interactionEvent(e);
}

uvec2 Canvas::mousePosToPixelCoordinates(ivec2 mpos) {
    uvec2 pos(std::max(mpos.x, 0), std::max(mpos.y, 0));
    pos = glm::min(pos, dimensions_);
    pos.y = dimensions_.y - pos.y;
    return pos;
}

} // namespace
