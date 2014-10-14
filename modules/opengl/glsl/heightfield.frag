/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Contact: Martin Falk
 *
 *********************************************************************************/

uniform sampler2D inportHeightfield_;
uniform sampler2D inportTexture_;
uniform sampler2D inportNormalMap_;

//uniform vec3 lightDir_ = vec3(-0.25, -0.25, -0.5);
uniform int terrainShadingMode_ = 0;
uniform int lighting_ = 0;

uniform vec3 lightIntensity_ = vec3(1.0);

in vec2 texCoord2D_;
in vec3 normal_;
in vec3 viewDir_;
in vec3 lightDir_;

in float height_;


float orenNayarDiffuseComplex(vec3 viewDir, vec3 lightDir, vec3 normal, float roughness=0.4)
{
// Oren-Nayar Illumination Model
// from http://content.gpwiki.org/index.php/D3DBook:(Lighting)_Oren-Nayar
    const float PI = 3.14159f;
    
    float tIn = dot(normal, lightDir);
    float tOut = dot(normal, viewDir);
    float thetaIn = acos(tIn);
    float thetaOut = acos(tOut);
    float alpha = max(thetaIn, thetaOut);
    float beta = min(thetaIn, thetaOut);
    float gamma = dot(abs(viewDir - normal * tOut), abs(lightDir - normal * tIn));
    float roughSq = roughness * roughness;
    
    float C1 = 1.0f - 0.5f * (roughSq / (roughSq + 0.33f));
    float C2 = 0.45f * (roughSq / (roughSq + 0.09f));
    if (gamma >= 0)
        C2 *= sin(alpha);
    else
        C2 *= (sin(alpha) - pow(2.0 * beta / PI, 3));
    
    float C3 = 1.0/8.0 * roughSq / (roughSq + 0.09f) * pow( 4.0f * alpha * beta / (PI * PI), 2);
    
    float A = gamma * C2 * tan(beta);
    float B = (1.0 - abs(gamma)) * C3 * tan((alpha + beta) * 0.5f);
    
    return max(0.0, tIn) * (C1 + A + B);
}


float orenNayarDiffuse(vec3 viewDir, vec3 lightDir, vec3 normal, float roughness=0.4)
{
// Oren-Nayar Illumination Model (simplified)
// from http://content.gpwiki.org/index.php/D3DBook:(Lighting)_Oren-Nayar
    const float PI = 3.14159f;
    
    float tIn = dot(normal, lightDir);
    float tOut = dot(normal, viewDir);
    float thetaIn = //acos(tIn);
        sqrt(max(0.0, 1.0 - tIn * tIn));
    float thetaOut = //acos(tOut);
        sqrt(max(0.0, 1.0 - tOut * tOut));
    float alpha = max(thetaIn, thetaOut);
    float beta = min(thetaIn, thetaOut);
    float gamma = dot(abs(viewDir - normal * tOut), abs(lightDir - normal * tIn));
    float roughSq = roughness * roughness;
    
    float A = 1.0f - 0.5f * (roughSq / (roughSq + 0.57f));
    float B = 0.45f * (roughSq / (roughSq + 0.09f));
    
    float C = sin(alpha) * tan(beta);
    
    float diffuse = A + B * max(0.0f, gamma) * C;
    
    return diffuse * max(0.0, tIn);
}


vec3 calcIllum(vec3 normal, vec3 viewDir, 
               vec3 lightDir, vec3 lightDirFill, vec3 srcColor)
{
    // main light source
    float ndotl = (dot(normal, lightDir));
    vec3 r = normalize(2.0 * ndotl * normal - lightDir);
    float spec = pow(max(dot(r, -viewDir), 0.0), 15.0) * 0.25; //10.0) * 0.1;
    float diff = max(abs(ndotl), 0.0);
    
    // fill light
    ndotl = dot(normal, lightDirFill);
    r = normalize(2.0 * ndotl * normal - lightDirFill);
    float specFill = pow(max(dot(r, -viewDir), 0.0), 10.0) * 0.14; // 20.0) * 0.14;
    float diffuseFill = 0.5*max(abs(ndotl), 0.0);
//    vec3 color = (diff + diffuseFill + 0.3)*srcColor.rgb + spec+specFill;

    vec3 color = (orenNayarDiffuse(-viewDir, lightDir, normal, 0.6)
        + 0.4*orenNayarDiffuse(-viewDir, lightDirFill, normal, 0.6) + 0.25) * srcColor.rgb + spec + specFill;

    //vec3 color = vec3(diff+ spec);

/*
    // illumination with light probes:
    r = normalize(2.0 * dot(normal, -dir) * normal + dir);
    vec3 diffHdr = textureCube(cubeMapDiffSampler, normal).rgb;
    vec3 specHdr = textureCube(cubeMapSpecSampler, -r).rgb;

    vec3 intensity = vec3(1.0) - exp(-diffHdr*0.8);
    vec3 color = (intensity*0.6 + 0.6)*srcColor.rgb;
    intensity = vec3(1.0) - exp(-specHdr*1.8);
    color += 0.1*intensity;//*srcColor.rgb;
*/
    return color;
}


void main() {

    vec4 color = gl_Color;
    
    if (terrainShadingMode_ == 1) { // color texture
        color = texture2D(inportTexture_, texCoord2D_).rgba;
    }
    else if (terrainShadingMode_ == 2) { // heightfield texture
        color = vec4(texture2D(inportHeightfield_, texCoord2D_).rrr, 1.0);
    }
/*	else {
        if (height_ < 0.1) 
            color = vec4(0.2, 0.2, 0.9, 1.0);
        else if (height_ < 0.2)
            color = vec4(0.4, 0.35, 0.1, 1.0);
        else if (height_ < 0.35)
            color = vec4(0.2, 0.5, 0.2, 1.0);
        else if (height_ < 1.0)
            color = vec4(0.5, 0.5, 0.5, 1.0);
    }
    */
    
    if (lighting_ > 0) {
        // sample normal map texture
        vec3 normal = normalize(texture2D(inportNormalMap_, texCoord2D_).rgb);
        //normal = normalize(normal_);
    
        vec3 lightDirFill = vec3(-0.4, 0.4, -0.5);
        color.rgb = calcIllum(normal, normalize(viewDir_), 
               normalize(-lightDir_), normalize(lightDirFill), color.rgb);
    }
    
    //color.rgb = normalize(normal_);
    FragData0 = color;
}