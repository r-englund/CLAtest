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

uniform sampler2D transferFunc_;

vec4 applyTF(sampler2D transferFunction, vec4 voxel) {
    return texture2D(transferFunction, vec2(voxel.a, 0.5));
}
