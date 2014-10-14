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
uniform float heightScale_ = 1.0f;
uniform vec3 lightPos_ = vec3(0.0, 0.0, -1.0);
uniform vec3 lightPos1_;
uniform vec3 lightPos2_;

out vec2 texCoord2D_;
out vec3 normal_;
out vec3 viewDir_;

out vec3 lightDir_;

out float height_;

void main() {
    gl_FrontColor = gl_Color;
    texCoord2D_ = gl_MultiTexCoord0.st;
    
    float height = texture2D(inportHeightfield_, texCoord2D_).r;
    
    height_ = height * heightScale_;
    vec4 pos = gl_Vertex + vec4(0.0f, 0.0f, height * heightScale_, 0.0f);
    vec4 posWorld = gl_ModelViewMatrix * pos;
    
    //normal_ = gl_Normal;
    normal_ = gl_NormalMatrix * gl_Normal;
    viewDir_ = posWorld.xyz; // posWorld
    
    // use directional lighting
    //lightDir_ = (gl_ModelViewMatrix * vec4(lightPos_, 0.0)).xyz;
    lightDir_ = lightPos_;
    
    gl_Position = gl_ProjectionMatrix * posWorld;
}