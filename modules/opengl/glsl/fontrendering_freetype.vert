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

layout(location = 0) in vec4 coord;

out vec2 texCoord;

void main(void) {
	gl_Position = vec4(coord.xy, 0, 1);
	texCoord = coord.zw;
}