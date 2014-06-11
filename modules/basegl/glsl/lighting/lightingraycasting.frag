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
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

#include "include/inc_sampler2d.frag"
#include "include/inc_sampler3d.frag"
#include "include/inc_raycasting.frag"
#include "include/inc_classification.frag"
#include "include/inc_gradients.frag"
#include "include/inc_shading.frag"
#include "include/inc_compositing.frag"
#include "include/inc_depth.frag"

uniform TEXTURE_TYPE entryColorTex_;
uniform TEXTURE_TYPE entryDepthTex_;
uniform TEXTURE_PARAMETERS entryParameters_;
uniform TEXTURE_TYPE exitColorTex_;
uniform TEXTURE_TYPE exitDepthTex_;
uniform TEXTURE_PARAMETERS exitParameters_;

uniform VOLUME_TYPE volume_;
uniform VOLUME_PARAMETERS volumeParameters_;

uniform VOLUME_TYPE lightVolume_;
uniform VOLUME_PARAMETERS lightVolumeParameters_;

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
        vec4 voxel = getNormalizedVoxel(volume_, volumeParameters_, samplePos);
        vec3 gradient = RC_CALC_GRADIENTS(voxel, samplePos, volume_, volumeParameters_, t, rayDirection, entryTex_, entryParameters_);
        vec4 color = RC_APPLY_CLASSIFICATION(transferFunc_, voxel);
        color.rgb = RC_APPLY_SHADING(color.rgb, color.rgb, vec3(1.0), samplePos, gradient, lightPosition_, cameraPosition_);
        //Light Volume Compositing
        vec4 lightVoxel = getNormalizedVoxel(lightVolume_, lightVolumeParameters_, samplePos);
#ifdef LIGHT_COLOR_ENABLED
        //color.rgb *= lightVoxel.rgb;
        color.rgb = (color.rgb * 0.6) + (lightVoxel.rgb * color.rgb * 0.8) + (0.8 * color.rgb * pow(lightVoxel.rgb, vec3(5.0)));
        color.rgb *= lightVoxel.a;
#else
        color.rgb *= lightVoxel.r;
#endif
        result = RC_APPLY_COMPOSITING(result, color, samplePos, voxel, gradient, t, tDepth, tIncr);

        // early ray termination
        if (result.a > ERT_THRESHOLD) t = tEnd;
        else t += tIncr;
    }

    if (tDepth != -1.0)
        tDepth = calculateDepthValue(tDepth, texture(entryDepthTex_, texCoords).z, texture(exitDepthTex_, texCoords).z);
    else
        tDepth = 1.0;

    gl_FragDepth = tDepth;
    return result;
}

void main() {
    vec2 texCoords = gl_FragCoord.xy * screenDimRCP_;
    vec3 entryPoint = texture(entryColorTex_, texCoords).rgb;
    vec3 exitPoint = texture(exitColorTex_, texCoords).rgb;

    if (entryPoint == exitPoint) discard;

    vec4 color = rayTraversal(entryPoint, exitPoint, texCoords);
    FragData0 = color;
}