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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#define VOLUME_TYPE sampler3D

struct VOLUME_PARAMETERS {
    vec3 dimensions_;
    vec3 dimensionsRCP_;
};

vec4 getVoxel(VOLUME_TYPE volume, VOLUME_PARAMETERS volumeParams, vec3 samplePos) {
    return texture3D(volume, samplePos);
}