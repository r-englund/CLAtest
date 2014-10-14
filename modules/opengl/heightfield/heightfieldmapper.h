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
 * Contact: Martin Falk
 *
 *********************************************************************************/

#ifndef IVW_HEIGHTFIELDMAPPER_H
#define IVW_HEIGHTFIELDMAPPER_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/datastructures/image/image.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/minmaxproperty.h>
#include <modules/opengl/inviwoopengl.h>


namespace inviwo {

// HeightFieldMapper converts an arbitrary input image to grayscale float image to be used 
// in heightfields. Additionally, data values are mapped to either an user-defined range 
// or are scaled to fit in a given maximum height based on the sea level.
class IVW_MODULE_OPENGL_API HeightFieldMapper : public Processor {
public:
    HeightFieldMapper();
    ~HeightFieldMapper();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    // adjust property visibility with respect to selected scaling mode
    void scalingModeChanged();

    enum ScalingMode {
        FIXED_RANGE, // scale to [0:1]
        DATA_RANGE, // scale heights given min/max values
        SEA_LEVEL,  // scale heights around sea level to fit in maxHeight
        NUMBER_OF_SCALING_MODES
    };

    ImageInport inport_;
    ImageOutport outport_;
        
    OptionPropertyInt scalingModeProp_;
    FloatMinMaxProperty heightRange_;
    FloatProperty maxHeight_;
    FloatProperty seaLevel_;
};

} // namespace

#endif // IVW_HEIGHTFIELDMAPPER_H
