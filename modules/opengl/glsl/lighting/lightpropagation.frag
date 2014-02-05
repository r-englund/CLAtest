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

#include "include/inc_sampler2d.frag"
#include "include/inc_sampler3d.frag"
#include "include/inc_classification.frag"

uniform VOLUME_TYPE volume_;
uniform VOLUME_PARAMETERS volumeParameters_;

uniform VOLUME_TYPE lightVolume_;
uniform VOLUME_PARAMETERS lightVolumeParameters_;

uniform vec4 permutedLightDirection_;

in vec4 texCoord_;
in vec4 permutedTexCoord_;

//Perform light propagation
vec4 propagateLight(in vec3 coord, in vec3 coordPerm) {
    //Retrieve voxel color
    vec4 voxel = getVoxel(volume_, volumeParameters_, coordPerm);
    vec4 color = applyTF(transferFunc_, voxel);
    //Calculate previous permuted coordinate (directional light)
    vec3 previousPermutedCoord = vec3(
                                     coord.xy - permutedLightDirection_.xy * lightVolumeParameters_.dimensionsRCP_.z,
                                     coord.z - lightVolumeParameters_.dimensionsRCP_.z);
    //Retrieve previous light value
    vec4 lightVoxel = getVoxel(lightVolume_, lightVolumeParameters_, previousPermutedCoord);
    //Return newly calculated propagate light value
    vec4 newCol = vec4((1.0 - color.a)*lightVoxel.a);
#ifdef SUPPORT_LIGHT_COLOR
    newCol.rgb = (1.0 - color.a)*lightVoxel.rgb + color.a*color.rgb;
#endif
    return newCol;
}

void main() {
    FragData0 = propagateLight(texCoord_.xyz, permutedTexCoord_.xyz);
}