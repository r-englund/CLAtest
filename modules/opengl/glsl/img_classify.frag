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

#include "modules/classification.frag"

uniform sampler2D inport_;
uniform vec2 dimension_;

void main() {
    vec2 texCoords = gl_FragCoord.xy * dimension_;
    vec4 value = texture2D(inport_, texCoords);
    vec4 color = applyTF(transferFunc_, value);
    color = value*vec4(1.0, 0.0, 0.0, 1.0)*5000;
    FragData0 = color;
}