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

uniform vec2 screenDim_;
uniform vec2 screenDimRCP_;

#define TEXTURE_TYPE sampler2D

struct TEXTURE_PARAMETERS {
    vec2 dimensions_;
    vec2 dimensionsRCP_;
};


vec4 textureLookup2D(TEXTURE_TYPE texture, TEXTURE_PARAMETERS textureParams, vec2 samplePos) {
    //deprecated
    return texture2D(texture, samplePos);
}

vec4 textureLookup2Dnormalized(TEXTURE_TYPE texture, TEXTURE_PARAMETERS textureParams, vec2 samplePos) {
    return texture2D(texture, samplePos);
}

vec4 textureLookup2Dscreen(TEXTURE_TYPE texture, TEXTURE_PARAMETERS textureParams, vec2 samplePos) {
    return texture2D(texture, samplePos*screenDimRCP_);
}