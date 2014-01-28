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

#include "modules/sampler2d.frag"

uniform TEXTURE_TYPE texColor0_;
uniform TEXTURE_TYPE texDepth0_;
uniform TEXTURE_TYPE texPicking0_;

uniform TEXTURE_TYPE texColor1_;
uniform TEXTURE_TYPE texDepth1_;
uniform TEXTURE_TYPE texPicking1_;

void main() {
    vec2 texCoords = gl_FragCoord.xy * screenDimRCP_;

    vec4 color0 = texture2D(texColor0_, texCoords);
    vec4 picking0 = texture2D(texPicking0_, texCoords);
    float depth0 = texture2D(texDepth0_, texCoords).z;

    vec4 color1 = texture2D(texColor1_, texCoords);
    vec4 picking1 = texture2D(texPicking1_, texCoords);
    float depth1 = texture2D(texDepth1_, texCoords).z;

    vec4 colorOut;
    vec4 pickingOut;
    float depthOut;

    if (depth1 <= depth0) {
        colorOut.rgb = color1.rgb + color0.rgb * color0.a * (1.0 - color1.a);
        colorOut.a = color1.a + color0.a * (1.0 - color1.a);
        pickingOut = (picking1.a > 0 ? picking1 : (color1.a < 0.95 ? picking0 : vec4(0.0)));
        depthOut = depth1;
    } else {
        colorOut.rgb = color0.rgb + color1.rgb * color1.a * (1.0 - color0.a);
        colorOut.a = color0.a + color1.a * (1.0 - color0.a);
        pickingOut = (picking0.a > 0 ? picking0 : (color0.a < 0.95 ? picking1 : vec4(0.0)));
        depthOut = depth0;
    }

    FragData0 = colorOut;
    PickingData = pickingOut;
    gl_FragDepth = depthOut;
}