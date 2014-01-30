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

#include "include/inc_sampler3d.frag"

uniform VOLUME_TYPE lightVolume_;
uniform VOLUME_TYPE lightVolumeSec_;

uniform VOLUME_PARAMETERS lightVolumeParameters_;

uniform float blendingFactor_;

in vec4 texCoord_;

in vec4 permutedPosInv_;
in vec4 permutedPosInvSec_;

void main() {    
    //Lookup light values
    vec4 lightValue = getVoxel(lightVolume_, lightVolumeParameters_, permutedPosInv_.xyz);
    vec4 lightValueSec = getVoxel(lightVolumeSec_, lightVolumeParameters_, permutedPosInvSec_.xyz);
    
    //Output blend between light volumes
    FragData0 = (lightValue * blendingFactor_) + (lightValueSec * (1.0 - blendingFactor_));
}