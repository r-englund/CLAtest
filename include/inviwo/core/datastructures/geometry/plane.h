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

#ifndef IVW_PLANE_H
#define IVW_PLANE_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API Plane {

public:
    Plane();
    Plane(vec3 point, vec3 normal);
    virtual ~Plane();

    vec3 getPoint() const;
    vec3 getNormal() const;

    vec3 getIntersection(const vec3&, const vec3&) const;

    vec3 projectPoint(const vec3&) const;

    bool isInside(const vec3&) const;

    bool perpendicularToPlane(const vec3&) const;

    void setPoint(const vec3);
    void setNormal(const vec3&);

private:
    vec3 point_;
    vec3 normal_;

};

} // namespace

#endif // IVW_PLANE_H