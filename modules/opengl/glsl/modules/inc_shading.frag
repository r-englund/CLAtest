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

uniform vec3 lightPosition_;

uniform vec3 lightColorAmbient_;
uniform vec3 lightColorDiffuse_;
uniform vec3 lightColorSpecular_;
uniform int lightSpecularExponent_;

vec3 shadeAmbient(vec3 colorAmb) {
    return colorAmb * lightColorAmbient_;
}

vec3 shadeDiffuse(vec3 colorDiff, vec3 gradient, vec3 lightPos) {
    return colorDiff * lightColorDiffuse_ * max(dot(gradient,lightPos), 0.0);
}

vec3 shadeSpecular(vec3 colorSpec, vec3 gradient, vec3 lightPos, vec3 cameraPos) {
    vec3 halfway = normalize(cameraPos + lightPos);
    return colorSpec * lightColorSpecular_ * pow(max(dot(gradient, halfway), 0.0), lightSpecularExponent_);
}

vec3 shadePhong(vec3 colorAmb, vec3 colorDiff, vec3 colorSpec, vec3 gradient, vec3 lightPos, vec3 cameraPos) {
    vec3 resAmb = shadeAmbient(colorAmb);
    vec3 resDiff = shadeDiffuse(colorDiff, gradient, lightPos);
    vec3 resSpec = shadeSpecular(colorSpec, gradient, lightPos, cameraPos);
    return resAmb + resDiff + resSpec;
}