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

#ifndef IVW_TRANSFERFUNCTIONDATAPOINT_H
#define IVW_TRANSFERFUNCTIONDATAPOINT_H

#include <stdlib.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/image/imagedisk.h>
#include <inviwo/core/datastructures/image/imageram.h>
#include <inviwo/core/ports/imageport.h>

namespace inviwo {

class IVW_CORE_API TransferFunctionDataPoint {

public:
	TransferFunctionDataPoint(vec2 pos, vec4 rgba);
    virtual ~TransferFunctionDataPoint();

    vec2 getPos() { return pos_; }
    void setPos(vec2 pos) { pos_ = pos; }

    vec4 getRGBA() { return rgba_; }
    void setRGBA(vec4 rgba) { rgba_ = rgba; }

    void setRGB(vec3 rgb);
    void setA(float alpha);

private:
	vec2 pos_;
    vec4 rgba_;
};

} // namespace

#endif // IVW_TRANSFERFUNCTIONDATAPOINT_H
