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

vec3 shadeAmbient(LIGHTING_PARAMETERS lighting, vec3 materialAmbientColor) {
    return materialAmbientColor * lighting.lightColor_;
}

vec3 shadeDiffuseCalculation(LIGHTING_PARAMETERS lighting, vec3 materialDiffuseColor, vec3 normal, vec3 toLightDir) {
    return materialDiffuseColor * lighting.lightColor_ * max(dot(normal, toLightDir), 0.0);
}

vec3 shadeSpecularCalculation(LIGHTING_PARAMETERS lighting, vec3 materialSpecularColor, vec3 normal, vec3 toLightDir,
                              vec3 toCameraDir) {
    vec3 halfway = normalize(toCameraDir + toLightDir);
    return materialSpecularColor * lighting.lightColor_ *
           pow(max(dot(normal, halfway), 0.0), lighting.materialSpecularExponent_);
}

// All positions and directions are assumed to be in world space!
vec3 shadeDiffuse(LIGHTING_PARAMETERS lighting, vec3 materialDiffuseColor,
                  vec3 position, vec3 normal) {
    return shadeDiffuseCalculation(lighting, materialDiffuseColor, normal,
                                   normalize(lighting.lightPosition_ - position));
}

vec3 shadeSpecular(LIGHTING_PARAMETERS lighting, vec3 materialSpecularColor,
                   vec3 position, vec3 normal, vec3 toCameraDir) {
    return shadeSpecularCalculation(lighting, materialSpecularColor, normal,
                                    normalize(lighting.lightPosition_ - position), 
                                    toCameraDir);
}

vec3 shadePhong(LIGHTING_PARAMETERS lighting, 
                vec3 materialAmbientColor, vec3 materialDiffuseColor, vec3 materialSpecularColor,
                vec3 position, vec3 normal, vec3 toCameraDir) {
    vec3 toLightDir = normalize(lighting.lightPosition_ - position);
    vec3 resAmb = shadeAmbient(lighting, materialAmbientColor); 
    vec3 resDiff = shadeDiffuse(lighting, materialDiffuseColor, normal, toLightDir);
    vec3 resSpec = shadeSpecularCalculation(lighting, materialSpecularColor, normal, toLightDir, toCameraDir);
    return resAmb + resDiff + resSpec;
}

#endif