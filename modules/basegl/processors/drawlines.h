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

#ifndef IVW_DRAWLINES_H
#define IVW_DRAWLINES_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/interaction/events/keyboardevent.h>
#include <inviwo/core/interaction/events/mouseevent.h>
#include <inviwo/core/ports/imageport.h>
#include <modules/opengl/image/compositeprocessorgl.h>
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <modules/opengl/rendering/meshrenderer.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/buttonproperty.h>

namespace inviwo {

class Shader;

//Hold Ctrl+D and click Left Mouse Button to Add Point
class IVW_MODULE_BASEGL_API DrawLines : public CompositeProcessorGL {
public:
    DrawLines();
    ~DrawLines();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

    bool isReady() const { return true; }

protected:
    void addPoint(vec2);
    void clearLines();

    void process();

    class DrawLinesInteractionHandler : public InteractionHandler {
    public:
        DrawLinesInteractionHandler(DrawLines* vs);
        ~DrawLinesInteractionHandler(){};

        void invokeEvent(Event* event);
    private:
        MouseEvent drawPosEvent;

        KeyboardEvent drawEnableEvent_;

        DrawLines* drawer_;

        bool drawModeEnabled_;
    };

private:
    ImageInport inport_;
    ImageOutport outport_;

    IntProperty lineSize_;
    FloatVec4Property lineColor_;
    ButtonProperty clearButton_;

    Mesh* lines_;
    MeshRenderer* lineRenderer_;

    Shader* lineShader_;
};

} // namespace

#endif // IVW_DRAWLINES_H
