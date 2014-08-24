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
 * Main file authors: Erik Sundén
 *
 *********************************************************************************/

#ifndef IVW_POINT_LIGHT_SOURCE_PROCESSOR_H
#define IVW_POINT_LIGHT_SOURCE_PROCESSOR_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/light/pointlight.h>
#include <inviwo/core/ports/dataoutport.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/interaction/trackball.h>

namespace inviwo {

class IVW_MODULE_BASE_API PointLightSourceProcessor : public Processor, public TrackballObserver {
public:
    PointLightSourceProcessor();
    virtual ~PointLightSourceProcessor();

    InviwoProcessorInfo();

protected:
    virtual void process();

    void onCameraChanged();
    void handleInteractionEventsChanged();
    virtual void onAllTrackballChanged(const Trackball* trackball);
    virtual void onLookFromChanged(const Trackball* trackball);
    virtual void onLookToChanged(const Trackball* trackball);
    virtual void onLookUpChanged(const Trackball* trackball);

    class PointLightInteractionHandler : public InteractionHandler {
    public:
        PointLightInteractionHandler(FloatVec3Property*, CameraProperty*);
        ~PointLightInteractionHandler(){};

        void invokeEvent(Event* event);
    private:
        FloatVec3Property* pointLight_;
        CameraProperty* camera_;
    };

    /**
     * Update light source parameters. Transformation will be given in texture space.
     *
     * @param lightSource
     * @return
     */
    void updatePointLightSource(PointLight* lightSource);

private:
    DataOutport<LightSource> outport_;

    FloatProperty lightPowerProp_;
    FloatProperty lightSize_;

    FloatVec4Property lightDiffuse_;
    FloatVec3Property lightPosition_;

    BoolProperty lightEnabled_;

    CameraProperty camera_;
    BoolProperty handleInteractionEvents_; ///< Enable or disable interactions from canvas

    Trackball* trackball_;
    vec3 lookUp_; ///< Necessary for trackball
    vec3 lookTo_; ///< Necessary for trackball
    PointLight* lightSource_;
};

} // namespace

#endif // IVW_POINT_LIGHT_SOURCE_PROCESSOR_H
