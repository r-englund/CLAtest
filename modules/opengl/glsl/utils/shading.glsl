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
#ifndef IVW_SHADING_GLSL
#define IVW_SHADING_GLSL

#include "utils/structs.glsl"

vec3 shadeAmbient(LIGHT_PARAMETERS light_, vec3 materialAmbientColor) {
    return materialAmbientColor * light_.ambientColor_;
}

vec3 shadeDiffuseCalculation(LIGHT_PARAMETERS light_, vec3 materialDiffuseColor, vec3 normal, vec3 toLightDir) {
    return materialDiffuseColor * light_.diffuseColor_ * max(dot(normal, toLightDir), 0.0);
}

vec3 shadeSpecularCalculation(LIGHT_PARAMETERS light_, vec3 materialSpecularColor, vec3 normal, vec3 toLightDir,
                              vec3 toCameraDir) {
    vec3 camDir = normalize(toCameraDir);
    vec3 halfway = camDir + toLightDir;

    // check for special case where the light source is exactly opposite
    // to the view direction, i.e. the length of the halfway vector is zero
    if (dot(halfway, halfway) < 1.0e-6) { // check for squared length
        return vec3(0.0);
    }
    else {
        halfway = normalize(halfway);
        return materialSpecularColor * light_.specularColor_ *
               pow(max(dot(normal, halfway), 0.0), light_.specularExponent_);
    }
}

// All positions and directions are assumed to be in world space!
vec3 shadeDiffuse(LIGHT_PARAMETERS light_, vec3 materialDiffuseColor,
                  vec3 position, vec3 normal) {
    return shadeDiffuseCalculation(light_, materialDiffuseColor, normal,
                                   normalize(light_.position_ - position));
}

vec3 shadeSpecular(LIGHT_PARAMETERS light_, vec3 materialSpecularColor,
                   vec3 position, vec3 normal, vec3 toCameraDir) {
    return shadeSpecularCalculation(light_, materialSpecularColor, normal,
                                    normalize(light_.position_ - position), 
                                    toCameraDir);
}

vec3 shadePhong(LIGHT_PARAMETERS light_, 
                vec3 materialAmbientColor, vec3 materialDiffuseColor, vec3 materialSpecularColor,
                vec3 position, vec3 normal, vec3 toCameraDir) {
    vec3 toLightDir = normalize(light_.position_ - position);
    vec3 resAmb = shadeAmbient(light_, materialAmbientColor); 
    vec3 resDiff = shadeDiffuseCalculation(light_, materialDiffuseColor, normal, toLightDir);
    vec3 resSpec = shadeSpecularCalculation(light_, materialSpecularColor, normal, toLightDir, toCameraDir);
    return resAmb + resDiff + resSpec;
}

#endif
