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

#ifndef IVW_CANVASPROCESSOR_H
#define IVW_CANVASPROCESSOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/directoryproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/util/canvas.h>

namespace inviwo {

class IVW_CORE_API CanvasProcessor : public Processor {
public:
    CanvasProcessor();
    ~CanvasProcessor();

    virtual void initialize();
    virtual void deinitialize();

    virtual void process();

    void setCanvas(Canvas* canvas) { canvas_ = canvas; }
    Canvas* getCanvas() const { return canvas_; }
    void setCanvasSize(ivec2);
    ivec2 getCanvasSize() const;

    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel,
                            Property* modifiedProperty=0);

    void saveImageLayer();
    void saveImageLayer(const char* filePath);

    void triggerQueuedEvaluation();

protected:
    void performEvaluationAtNextShow();
    void performEvaluateRequest();

    ImageInport inport_;

    IntVec2Property dimensions_;
    IntOptionProperty visibleLayer_; // LayerType enum (Cannot serialize/deserialize enums so we use an int and cast it)
    DirectoryProperty saveLayerDirectory_;
    ButtonProperty saveLayerButton_;

private:
    Canvas* canvas_;
    bool disableResize_;
    bool queuedRequest_;

    void resizeCanvas();
};

} // namespace

#endif // IVW_CANVASPROCESSOR_H
