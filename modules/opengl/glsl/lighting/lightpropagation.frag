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
vec4 propagateLight(in vec3 coord, in vec3 coordPerm){
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