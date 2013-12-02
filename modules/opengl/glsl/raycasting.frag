/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include "modules/mod_sampler2d.frag"
#include "modules/mod_sampler3d.frag"
#include "modules/mod_raycasting.frag"
#include "modules/mod_classification.frag"
#include "modules/mod_gradients.frag"
#include "modules/mod_shading.frag"
#include "modules/mod_compositing.frag"
#include "modules/mod_depth.frag"

uniform TEXTURE_TYPE entryColorTex_;
uniform TEXTURE_TYPE entryDepthTex_;
uniform TEXTURE_PARAMETERS entryParameters_;
uniform TEXTURE_TYPE exitColorTex_;
uniform TEXTURE_TYPE exitDepthTex_;
uniform TEXTURE_PARAMETERS exitParameters_;

uniform VOLUME_TYPE volume_;
uniform VOLUME_PARAMETERS volumeParameters_;

uniform vec2 dimension_;
uniform vec3 volumeDimension_;

// set threshold for early ray termination
#define ERT_THRESHOLD 0.95

vec4 rayTraversal(vec3 entryPoint, vec3 exitPoint, vec2 texCoords) {
    vec4 result = vec4(0.0);

    float t = 0.0;
    vec3 rayDirection = exitPoint - entryPoint;
    float tIncr = 1.0/(samplingRate_*length(rayDirection*volumeParameters_.dimensions_));
    float tEnd = length(rayDirection);
    rayDirection = normalize(rayDirection);
    float tDepth = -1.0;

    while (t < tEnd) {
        vec3 samplePos = entryPoint + t * rayDirection;
        vec4 voxel = getVoxel(volume_, volumeParameters_, samplePos);
        voxel.xyz = RC_CALC_GRADIENTS(voxel, samplePos, volume_, volumeParameters_, t, rayDirection, entryTex_, entryParameters_);

        vec4 color = RC_APPLY_CLASSIFICATION(transferFunc_, voxel);

        color.rgb = RC_APPLY_SHADING(color.rgb, color.rgb, vec3(1.0), voxel.xyz, lightPosition_, cameraPosition_);

        result = RC_APPLY_COMPOSITING(result, color, samplePos, voxel.xyz, t, tDepth, tIncr);

        // early ray termination
        if (result.a > ERT_THRESHOLD) t = tEnd;
        else t += tIncr;
    }

    if (tDepth != -1.0)
        tDepth = calculateDepthValue(tDepth, texture2D(entryDepthTex_, texCoords).z, texture2D(exitDepthTex_, texCoords).z);
    else
        tDepth = 1.0;

    gl_FragDepth = tDepth;

    return result;
}

void main() {
    vec2 texCoords = gl_FragCoord.xy * screenDimRCP_;
    vec3 entryPoint = texture2D(entryColorTex_, texCoords).rgb;
    vec3 exitPoint = texture2D(exitColorTex_, texCoords).rgb;
    if (entryPoint == exitPoint) discard;
    vec4 color = rayTraversal(entryPoint, exitPoint, texCoords);
    FragData0 = color;
}