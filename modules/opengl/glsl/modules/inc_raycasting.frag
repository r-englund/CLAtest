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

#define REF_SAMPLING_INTERVAL 150.0
uniform float samplingRate_;

uniform vec3 cameraPosition_;

uniform float isoValue_;

void raySetup(in vec3 entryPoint, in vec3 exitPoint, in vec3 volDimensions, out vec3 rayDirection, out float tIncr, out float tEnd) {
    rayDirection = exitPoint - entryPoint;
    tEnd = length(rayDirection);
    rayDirection = normalize(rayDirection);
    tIncr = 1.0/(samplingRate_*length(rayDirection*volDimensions));
}