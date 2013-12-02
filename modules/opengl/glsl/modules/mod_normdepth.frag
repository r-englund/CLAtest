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

uniform float minDepth_;
uniform float maxDepth_;

float normalizeDepth(float depth) {
    return depth;
    float result = depth - minDepth_;
    result /= (maxDepth_ - minDepth_);
    return result;
}

float normDepth(float depth) {
    // deprecated
    return normalizeDepth(depth);
}

float calculateDepthValue(float t, float minDepth, float maxDepth) {
    // FIXME: implement correct depth value caluclation
    return (minDepth + (t*(maxDepth-minDepth)));
}