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

#include <inviwo/core/ports/geometryport.h>
#include <inviwo/core/datastructures/geometry/simplemesh.h>

namespace inviwo {

uvec3 GeometryInport::colorCode = uvec3(188,188,101);

// Geometry Inport
GeometryInport::GeometryInport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel)
    : DataInport<Geometry>(identifier, invalidationLevel)
{}

GeometryInport::~GeometryInport() {
}

void GeometryInport::initialize() {}

void GeometryInport::deinitialize() {}


uvec3 GeometryInport::getColorCode() const {
    return GeometryInport::colorCode;
}

GeometryMultiInport::GeometryMultiInport(std::string identifier)
    : MultiDataInport<Geometry>(identifier)
{}

GeometryMultiInport::~GeometryMultiInport() {
}

void GeometryMultiInport::initialize() {}

void GeometryMultiInport::deinitialize() {}


uvec3 GeometryMultiInport::getColorCode() const {
    return GeometryInport::colorCode;
}

// Geometry Outport
GeometryOutport::GeometryOutport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel)
    : DataOutport<Geometry>(identifier, invalidationLevel)
{}

GeometryOutport::~GeometryOutport() {}

void GeometryOutport::initialize() {}

void GeometryOutport::deinitialize() {}

uvec3 GeometryOutport::getColorCode() const {
    return GeometryInport::colorCode;
}

} // namespace