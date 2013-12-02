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

uniform sampler2D inport_;
uniform vec2 dimension_;

void main() {
    vec2 texCoords = gl_FragCoord.xy * dimension_; //TODO: replace vec2(512.0) by screenDimRCP
    vec3 inputColor = texture2D(inport_, texCoords).rgb;
    float gray = 0.2989*inputColor.r + 0.5870*inputColor.g + 0.1140*inputColor.b;
    vec4 color = vec4(gray, gray, gray, 1.0);
    FragData0 = color;
}