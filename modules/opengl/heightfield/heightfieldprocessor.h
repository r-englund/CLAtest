/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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

#ifndef IVW_HEIGHTFIELDPROCESSOR_H
#define IVW_HEIGHTFIELDPROCESSOR_H

#include <modules/opengl/openglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/dataoutport.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/datastructures/baselightsource.h>

#include <modules/opengl/geometry/geometryrenderprocessorgl.h>
#include <modules/opengl/glwrap/shader.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/templateproperty.h>
#include <vector>

namespace inviwo {

class IVW_MODULE_OPENGL_API HeightFieldProcessor : public GeometryRenderProcessorGL {
public:
    HeightFieldProcessor();
    ~HeightFieldProcessor();

    InviwoProcessorInfo();

    virtual bool isReady() const {
        // only the mesh input port is mandatory
        return inport_.isReady(); 
    }

    virtual void initialize();

protected:
    virtual void process();

private:
    enum ShadingType { HF_SHADING_FLAT, 
        HF_SHADING_COLORTEX, 
        HF_SHADING_HEIGHTFIELD, 
        NUMBER_OF_HF_SHADING_MODES };

    void setupLight();
    void lightingChanged();
    void heightfieldChanged();

    ImageInport inportHeightfield_;
    ImageInport inportTexture_;
    ImageInport inportNormalMap_;
    DataInport<LightSource> inportLightSource_;

    FloatProperty heightScale_; // Scale height of input image
    OptionPropertyInt terrainShadingMode_;
    BoolProperty lightingEnabledProperty_;

    bool lighting_;
};

} // namespace

#endif // IVW_HEIGHTFIELDPROCESSOR_H
