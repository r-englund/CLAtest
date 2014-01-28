/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
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

#include "include/inc_sampler2d.frag"

uniform sampler2D inport0_;
uniform sampler2D inport1_;
uniform float alpha_;
uniform vec2 dimension_;

void main() {
    vec2 texCoords = gl_FragCoord.xy * screenDimRCP_;
    vec3 color0 = texture2D(inport0_, texCoords).rgb;
    vec3 color1 = texture2D(inport1_, texCoords).rgb;
    vec4 color = vec4(mix(color0, color1, alpha_), 1.0);
    FragData0 = color;
    gl_FragDepth = 0.0;
}