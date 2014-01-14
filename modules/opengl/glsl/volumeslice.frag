/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
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

#include "modules/mod_sampler2d.frag"
#include "modules/mod_sampler3d.frag"
#include "modules/mod_classification.frag"

uniform VOLUME_TYPE volume_;
uniform VOLUME_PARAMETERS volumeParameters_;

uniform vec2 dimension_;
uniform float sliceNum_;

void main() {
    vec2 texC = gl_FragCoord.xy * screenDimRCP_;
    vec4 voxel = getVoxel(volume_, volumeParameters_, vec3(coordPlanePermute(texC.x, texC.y, sliceNum_)));
    vec4 color = applyTF(transferFunc_, voxel);
    FragData0 = color;
}