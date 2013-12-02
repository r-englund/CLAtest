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

#include <inviwo/core/datastructures/geometry/geometryrepresentation.h>

namespace inviwo {

GeometryRepresentation::GeometryRepresentation()
    : DataRepresentation()
{}

GeometryRepresentation::~GeometryRepresentation(){}

void GeometryRepresentation::performOperation(DataOperation*) const {}

std::string GeometryRepresentation::getClassName() const { 
    return "GeometryRepresentation"; 
}


} // namespace
