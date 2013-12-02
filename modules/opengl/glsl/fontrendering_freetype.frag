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
 * Primary author : Johan Noren
 *
 **********************************************************************/

uniform sampler2D texture;
uniform vec4 color;

in vec2 texCoord;

void main(void) {
	FragData0 = vec4(1.0f,1.0f,1.0f, texture2D(texture, texCoord).a) * color;
	gl_FragDepth = 0.0;
}