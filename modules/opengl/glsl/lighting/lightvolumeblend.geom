/**********************************************************************
 * Copyright (C) 2013-2014 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#extension GL_EXT_gpu_shader4: enable
#extension GL_EXT_geometry_shader4: enable
#extension GL_EXT_geometry_shader4: enable

#include "include/inc_sampler3d.frag"

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform VOLUME_PARAMETERS lightVolumeParameters_;

uniform mat4 permMatInv_;
uniform mat4 permMatInvSec_;

in int instanceID_[3];
in vec2 texCoord2D_[3];

out vec4 texCoord_;

out vec4 permutedPosInv_;
out vec4 permutedPosInvSec_;

void main() {
    int i;
    texCoord_.z = (instanceID_[0] * lightVolumeParameters_.dimensionsRCP_.z) + (0.5 * lightVolumeParameters_.dimensionsRCP_.z);
    texCoord_.w = 1.f;
    gl_Layer = instanceID_[0];

	for (i = 0; i<gl_VerticesIn; ++i){
		gl_Position = gl_PositionIn[i];
        texCoord_.xy = texCoord2D_[i];
        permutedPosInv_ = permMatInv_ * texCoord_;
        permutedPosInvSec_ = permMatInvSec_ * texCoord_;
		EmitVertex();
	}
	EndPrimitive();
}