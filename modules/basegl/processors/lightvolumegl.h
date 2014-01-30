/**********************************************************************
 * Copyright (C) 2013-2014 Scientific Visualization Group - Linköping University
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

#ifndef IVW_LIGHTVOLUMEGL_H
#define IVW_LIGHTVOLUMEGL_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <modules/opengl/processorgl.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/ports/datainport.h>
#include <inviwo/core/datastructures/baselightsource.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <modules/opengl/glwrap/framebufferobject.h>
#include <modules/opengl/volume/volumegl.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API LightVolumeGL : public ProcessorGL {
public:
    LightVolumeGL();
    ~LightVolumeGL();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

    void propagation3DTextureParameterFunction(Texture*);

protected:
    void process();

    struct PropagationParameters {
        FrameBufferObject* fbo;
        VolumeGL* vol;
        mat4 axisPermutation;
        mat4 axisPermutationINV;
        mat4 axisPermutationLight;
        vec4 permutedLightDirection;

        PropagationParameters() : fbo(NULL), vol(NULL) {}
        ~PropagationParameters() { delete fbo; delete vol; };
    };

    bool lightSourceChanged();
    bool volumeChanged(bool);

    void volumeSizeOptionChanged();
    void supportColoredLightChanged();

    void borderColorTextureParameterFunction();

    void updatePermuationMatrices(const vec3&, PropagationParameters*, PropagationParameters*);

private:
    VolumeInport inport_;
    VolumeOutport outport_;

    DataInport<LightSource> lightSource_;

    BoolProperty supportColoredLight_;
    OptionPropertyInt volumeSizeOption_;
    TransferFunctionProperty transferFunction_;

    Shader* propagationShader_;
    Shader* mergeShader_;

    PropagationParameters propParams_[2];
    
    FrameBufferObject* mergeFBO_;
    float blendingFactor_;

    bool internalVolumesInvalid_;
    uvec3 volumeDimOut_;
    vec3 volumeDimOutF_;
    vec3 volumeDimOutFRCP_;
    vec3 volumeDimInF_;
    vec3 volumeDimInFRCP_;
    vec3 lightPos_;
    vec4 lightColor_;
    bool calculatedOnes_;
};

} // namespace

#endif // IVW_LIGHTVOLUMEGL_H
