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

uniform sampler2D color_;
uniform sampler2D depth_;
uniform sampler2D picking_;

void main() {
    FragData0 = texture2D(color_, gl_TexCoord[0].st);
    PickingData = texture2D(picking_, gl_TexCoord[0].st);
    gl_FragDepth = texture2D(depth_, gl_TexCoord[0].st).r;
}