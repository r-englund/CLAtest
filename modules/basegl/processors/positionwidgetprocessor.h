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

#ifndef IVW_POSITIONWIDGETPROCESSOR_H
#define IVW_POSITIONWIDGETPROCESSOR_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/geometryport.h>
#include <modules/opengl/image/compositeprocessorgl.h>
#include <inviwo/core/interaction/pickingobject.h>
#include <inviwo/core/properties/cameraproperty.h>


namespace inviwo {

class IVW_MODULE_BASEGL_API PositionWidgetProcessor : public CompositeProcessorGL {
public:
    PositionWidgetProcessor();
    ~PositionWidgetProcessor();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

    bool isReady() const { return geometryInport_.isReady(); }

    void updateWidgetPositionFromPicking(const PickingObject*);

protected:
    virtual void process();

private:
    GeometryInport geometryInport_;
    ImageInport imageInport_;
    ImageOutport outport_;

    FloatVec3Property position_;

    CameraProperty camera_;

    const PickingObject* widgetPickingObject_;

    Shader* shader_;
};

} // namespace

#endif // IVW_POSITIONWIDGETPROCESSOR_H
