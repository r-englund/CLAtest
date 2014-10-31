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
 * Contact: Rickard Englund
 *
 *********************************************************************************/

#include "utils/structs.glsl"
#include "utils/sampler2d.glsl"
#include "utils/sampler3d.glsl"

#include "utils/classification.glsl"
#include "utils/compositing.glsl"
#include "utils/depth.glsl"
#include "utils/gradients.glsl"
#include "utils/shading.glsl"

uniform VOLUME_PARAMETERS volumeParameters_;
uniform sampler3D volume_;

uniform sampler2D transferFunc_;

uniform TEXTURE_TYPE entryColorTex_;
uniform TEXTURE_TYPE entryDepthTex_;
uniform TEXTURE_PARAMETERS entryParameters_;

uniform TEXTURE_TYPE exitColorTex_;
uniform TEXTURE_TYPE exitDepthTex_;
uniform TEXTURE_PARAMETERS exitParameters_;

uniform TEXTURE_PARAMETERS outportParameters_;

uniform SHADING_PARAMETERS light_;
uniform CAMERA_PARAMETERS camera_;
uniform int channel_;

uniform float samplingRate_;
uniform float isoValue_;

#define ERT_THRESHOLD 0.95 // set threshold for early ray termination

vec4 rayTraversal(vec3 entryPoint, vec3 exitPoint, vec2 texCoords) {
    vec4 result = vec4(0.0);
    vec3 rayDirection = exitPoint - entryPoint;
    float tEnd = length(rayDirection);
    float tIncr = min(tEnd, tEnd / (samplingRate_*length(rayDirection*volumeParameters_.dimensions_)));
    float samples = ceil(tEnd/tIncr);
    tIncr = tEnd/samples;
    float t = 0.5f*tIncr; 
    rayDirection = normalize(rayDirection);
    float tDepth = -1.0;
    vec4 color; vec4 voxel;
    vec3 samplePos; vec3 gradient;
    float prevS = 0;
    
    if(t >= tEnd){
        gl_FragDepth = tDepth;
        return result; 
    }

    samplePos = entryPoint + t * rayDirection;
    bool outside = getNormalizedVoxel(volume_, volumeParameters_, samplePos)[channel_] < isoValue_;
    t += tIncr;

    int stop = 1000;
    while (t < tEnd && stop-- > 0) {
        samplePos = entryPoint + t * rayDirection;
        voxel = getNormalizedVoxel(volume_, volumeParameters_, samplePos);
        
        float diff = voxel[channel_] - isoValue_;
        bool sampOutside = voxel[channel_] < isoValue_;
        float th = 0.001;
        if (abs(diff) < th) { //close enough to the surface
            gradient = COMPUTE_GRADIENT_FOR_CHANNEL(voxel, volume_, volumeParameters_, samplePos, channel_);
            result.rgb = APPLY_LIGHTING(light_, camera_, volumeParameters_, vec3(1.0), vec3(1.0),
                                        vec3(1.0), samplePos, gradient);
            result.a = 1.0;
            t += tEnd;
            break;
        } else if(sampOutside != outside) {
            t -= tIncr;
            tIncr /= 2.0;
        }
        // early ray termination
        if (result.a > ERT_THRESHOLD) {
            t = tEnd;
        } else {
            t += tIncr;
		}
    }

    if (tDepth != -1.0) {
        tDepth = calculateDepthValue(camera_, tDepth, texture(entryDepthTex_, texCoords).z,
                                     texture(exitDepthTex_, texCoords).z);
    } else {
        tDepth = 1.0;
	}

    gl_FragDepth = tDepth;
    return result;
}

void main() {
    vec2 texCoords = gl_FragCoord.xy * outportParameters_.dimensionsRCP_;
    vec3 entryPoint = texture(entryColorTex_, texCoords).rgb;
    vec3 exitPoint = texture(exitColorTex_, texCoords).rgb;

    if (entryPoint == exitPoint) discard;

    vec4 color = rayTraversal(entryPoint, exitPoint, texCoords);
    FragData0 = color;
}