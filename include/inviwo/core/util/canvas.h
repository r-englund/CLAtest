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
 * Main file authors: Erik Sundén, Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_CANVAS_H
#define IVW_CANVAS_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/image.h>
#include <inviwo/core/datastructures/image/imagetypes.h>
#include <inviwo/core/datastructures/geometry/geometry.h>
#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/interaction/pickingcontainer.h>
#include <inviwo/core/interaction/events/eventhandler.h>
#include <inviwo/core/interaction/events/gestureevent.h>
#include <inviwo/core/interaction/events/keyboardevent.h>
#include <inviwo/core/interaction/events/mouseevent.h>
#include <inviwo/core/interaction/events/touchevent.h>

namespace inviwo {

class IVW_CORE_API Canvas {

    friend class CanvasProcessor;
    friend class ProcessorNetworkEvaluator;

public:
    Canvas(uvec2 dimensions);
    virtual ~Canvas();

    virtual void initialize();
    virtual void deinitialize();
    virtual void activate();
    virtual void render(const Image*, LayerType layerType = COLOR_LAYER);
    virtual void resize(uvec2 canvasSize, uvec2 imageSize);

    uvec2 getImageDimension();
    uvec2 getScreenDimension();

    virtual void update();

    bool isInitialized();

protected:
    void setNetworkEvaluator(ProcessorNetworkEvaluator* networkEvaluator);
    ProcessorNetworkEvaluator* getProcessorNetworkEvaluator();

    void activateDefaultRenderContext();

    void interactionEvent(InteractionEvent* e);

    void mousePressEvent(MouseEvent* e);
    void mouseReleaseEvent(MouseEvent* e);
    void mouseMoveEvent(MouseEvent* e);
    void mouseWheelEvent(MouseEvent* e);

    void keyPressEvent(KeyboardEvent* e);
    void keyReleaseEvent(KeyboardEvent* e);

    void gestureEvent(GestureEvent* e);
    void touchEvent(TouchEvent* e);

    uvec2 mousePosToPixelCoordinates(ivec2 mpos);

    static Geometry* screenAlignedRect_;

    bool initialized_;
    bool shared_;
    uvec2 screenDimensions_;
    uvec2 imageDimensions_;

    PickingContainer* pickingContainer_;

    ProcessorNetworkEvaluator* processorNetworkEvaluator_;
};

} // namespace

#endif // IVW_CANVAS_H
