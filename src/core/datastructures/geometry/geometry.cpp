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

#include <inviwo/core/datastructures/geometry/geometry.h>
#include <inviwo/core/datastructures/geometry/mesh.h>

namespace inviwo {

Geometry::Geometry() : SpatialData<3>() {}

Geometry::Geometry(GeometryRepresentation* rep) : SpatialData<3>() {
    addRepresentation(rep);
}

Geometry* Geometry::clone() const {
    return new Geometry(*this);
}

Geometry::~Geometry() {
}

DataRepresentation* Geometry::createDefaultRepresentation() {
    return NULL;
    //return new Mesh();  //Peter, Mesh is not a DataRepresentation? what to do?
}

} // namespace
