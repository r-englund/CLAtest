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
 * Primary author : Viktor Axelsson
 *
 **********************************************************************/

#include <inviwo/core/datastructures/transferfunctiondatapoint.h>

namespace inviwo {

TransferFunctionDataPoint::TransferFunctionDataPoint(vec2 pos, vec4 rgba)
    : pos_(pos)
    , rgba_(rgba)
{}

TransferFunctionDataPoint::~TransferFunctionDataPoint() {}

void TransferFunctionDataPoint::setRGB(vec3 rgb) {
    rgba_.r = rgb.r;
    rgba_.g = rgb.g;
    rgba_.b = rgb.b;
}

void TransferFunctionDataPoint::setA(float alpha) {
    rgba_.a = alpha;
}

} // namespace