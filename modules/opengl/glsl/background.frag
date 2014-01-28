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

#include "include/inc_sampler2d.frag"

uniform TEXTURE_TYPE srcColorTex_;
uniform TEXTURE_PARAMETERS srcColorParameters_;

uniform int backgroundStyle_;
uniform vec4 color1_;
uniform vec4 color2_;

vec4 checkerBoard(vec2 texCoords) {
	if ((int(floor(texCoords.x*10.0) + floor(texCoords.y*10.0)) & 1) == 0)
		return color1_;
	else
		return color2_;
}

vec4 linearGradient(vec2 texCoords) {
	return texCoords.y*color1_ + (1.0-texCoords.y)*color2_;
}

void main() {
    vec2 texCoords = gl_FragCoord.xy * screenDimRCP_;
    vec4 srcColor = texture2D(srcColorTex_, texCoords);

	vec4 backgroundColor = BACKGROUND_STYLE_FUNCTION;

	vec4 resultColor;
    resultColor.rgb = srcColor.rgb + backgroundColor.rgb * backgroundColor.a * (1.0 - srcColor.a);
    resultColor.a = srcColor.a + backgroundColor.a * (1.0 - srcColor.a);
    FragData0 = resultColor;
}