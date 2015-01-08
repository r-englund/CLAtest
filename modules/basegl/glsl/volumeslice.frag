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
 * Contact: Erik Sundén
 *
 *********************************************************************************/

#include "utils/structs.glsl"
#include "utils/sampler3d.glsl"
#include "utils/classification.glsl"

uniform sampler3D volume_;
uniform VolumeParameters volumeParameters_;

uniform sampler2D transferFunc_;

uniform vec2 dimension_;
uniform float sliceNum_;

uniform mat4 sliceAxisRotationMatrix_; // Rotates around slice axis (offset to center point)
uniform vec3 rotationOffset_;          // Translates coordinate back from rotation offset
uniform float alphaOffset_ = 0.0;

in vec3 texCoord_;

void main() {
#ifdef COORD_PLANE_PERMUTE
    vec3 p = vec3(coordPlanePermute(texCoord_.x, texCoord_.y, sliceNum_));
#else
    vec3 p = vec3(texCoord_.x, texCoord_.y, sliceNum_);
#endif
    // Rotate around center and translate back to origin
    vec3 pRotated = (sliceAxisRotationMatrix_*vec4(p, 1.0)).xyz + rotationOffset_;
    vec4 voxel = getNormalizedVoxel(volume_, volumeParameters_, pRotated);
#ifdef TF_MAPPING_ENABLED
    voxel = applyTF(transferFunc_, voxel);
    voxel.a += alphaOffset_;
#endif
    FragData0 = voxel;

}