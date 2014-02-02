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

class TransferFunction;

class IVW_CORE_API TransferFunctionDataPoint {

public:
    TransferFunctionDataPoint(TransferFunction* transFunc, const vec2& pos, const vec4& rgba);
    virtual ~TransferFunctionDataPoint();

    const vec2& getPos() const;
    void setPos(const vec2& pos);

    const vec4& getRGBA() const;
    void setRGBA(const vec4& rgba);

    void setRGB(const vec3& rgb);
    void setA(float alpha);

    void setPosA(const vec2& pos, float alpha);

private:
    vec2 pos_;
    vec4 rgba_;
    TransferFunction* transFunc_;
};

} // namespace

#endif // IVW_TRANSFERFUNCTIONDATAPOINT_H
