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

vec3 shadeAmbient(vec3 colorAmb) {
    return colorAmb * lightColorAmbient_;
}

vec3 shadeDiffuse(vec3 colorDiff, vec3 gradient, vec3 lightPos) {
    return colorDiff * lightColorDiffuse_ * max(dot(normalize(gradient),lightPos), 0.0);
}

vec3 shadeSpecular(vec3 colorSpec, vec3 gradient, vec3 lightPos, vec3 cameraPos) {
    vec3 halfway = normalize(cameraPos + lightPos);
    return colorSpec * lightColorSpecular_ * pow(max(dot(normalize(gradient), halfway), 0.0), lightSpecularExponent_);
}

vec3 shadePhong(vec3 colorAmb, vec3 colorDiff, vec3 colorSpec, vec3 gradient, vec3 lightPos, vec3 cameraPos) {
    vec3 resAmb = shadeAmbient(colorAmb);
    vec3 resDiff = shadeDiffuse(colorDiff, gradient, lightPos);
    vec3 resSpec = shadeSpecular(colorSpec, gradient, lightPos, cameraPos);
    return resAmb + resDiff + resSpec;
}