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
 * Main file author: Timo Ropinski
 *
 *********************************************************************************/

uniform vec3 lightPosition_;

uniform vec3 lightColorAmbient_;
uniform vec3 lightColorDiffuse_;
uniform vec3 lightColorSpecular_;
uniform int lightSpecularExponent_;

uniform mat4 viewToVoxel_;
uniform mat4 viewMatrix_;


vec3 shadeAmbient(vec3 colorAmb) {
    return colorAmb * lightColorAmbient_;
}

vec3 shadeDiffuseCalculation(vec3 colorDiff, vec3 normal, vec3 lightDir) {
    return colorDiff * lightColorDiffuse_ * max(dot(normal,lightDir), 0.0);
}

vec3 shadeSpecularCalculation(vec3 colorSpec, vec3 normal, vec3 lightDir, vec3 cameraDir) {
    vec3 halfway = normalize(cameraDir + lightDir);
    return colorSpec * lightColorSpecular_ * pow(max(dot(normal, halfway), 0.0), lightSpecularExponent_);
}

vec3 shadeDiffuse(vec3 colorDiff, vec3 position, vec3 gradient, vec3 lightPos) {
    vec3 lightInVoxelSpace = (viewToVoxel_ * vec4(vec3(-lightPos),1.0)).xyz;
    vec3 gradientInViewSpace = (viewMatrix_ * vec4(gradient,0.0)).xyz;
    return shadeDiffuseCalculation(colorDiff,
                                   normalize(gradientInViewSpace),
                                   normalize(lightInVoxelSpace - position));
}


vec3 shadeSpecular(vec3 colorSpec, vec3 position, vec3 gradient, vec3 lightPos, vec3 cameraPos) {
    vec3 lightInVoxelSpace = (viewToVoxel_ * vec4(vec3(-lightPos),1.0)).xyz;
    vec3 cameraInVoxelSpace = (viewToVoxel_ * vec4(cameraPos,1.0)).xyz;
    vec3 gradientInViewSpace = (viewMatrix_ * vec4(gradient,0.0)).xyz;
    return shadeSpecularCalculation(colorSpec,
                                    normalize(gradientInViewSpace),
                                    normalize(lightInVoxelSpace  - position),
                                    normalize(cameraInVoxelSpace - position));
}

vec3 shadePhong(vec3 colorAmb, vec3 colorDiff, vec3 colorSpec, vec3 position, vec3 gradient, vec3 lightPos, vec3 cameraPos) {
    vec3 lightInVoxelSpace = (viewToVoxel_ * vec4(vec3(-lightPos),1.0)).xyz;
    vec3 cameraInVoxelSpace = (viewToVoxel_ * vec4(cameraPos,1.0)).xyz;
    vec3 gradientInViewSpace = (viewMatrix_ * vec4(gradient,0.0)).xyz;
    vec3 normal = normalize(gradientInViewSpace);
    vec3 lightDir = normalize(lightInVoxelSpace - position);
    vec3 cameraDir = normalize(cameraInVoxelSpace - position);
    vec3 resAmb = shadeAmbient(colorAmb);
    vec3 resDiff = shadeDiffuseCalculation(colorDiff, normal, lightDir);
    vec3 resSpec = shadeSpecularCalculation(colorSpec, normal, lightDir, cameraDir);
    return resAmb + resDiff + resSpec;
}
