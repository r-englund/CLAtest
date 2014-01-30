/**********************************************************************
 * Copyright (C) 2013-2014 Scientific Visualization Group - Linköping University
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
 
out vec2 texCoord2D_;
out int instanceID_;

void main() {
    texCoord2D_ = gl_MultiTexCoord0.xy;
    instanceID_ = gl_InstanceID;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}