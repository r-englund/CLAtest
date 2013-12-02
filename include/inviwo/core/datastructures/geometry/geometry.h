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

#ifndef IVW_GEOMETRY_H
#define IVW_GEOMETRY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/spatialdata.h>
#include <inviwo/core/datastructures/geometry/geometryrepresentation.h>

namespace inviwo {

class IVW_CORE_API Geometry : public SpatialData<3> {

public:
    Geometry();
    Geometry(GeometryRepresentation* rep);
    virtual ~Geometry();
    virtual Geometry* clone() const;

protected:
    virtual DataRepresentation* createDefaultRepresentation();
};

} // namespace

#endif // IVW_GEOMETRY_H
