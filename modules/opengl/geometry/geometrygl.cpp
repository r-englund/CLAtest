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

#include <modules/opengl/geometry/geometrygl.h>

namespace inviwo {

GeometryGL::GeometryGL()
    : GeometryRepresentation(){
}

GeometryGL::GeometryGL(const GeometryGL& rhs) 
    : GeometryRepresentation(rhs){
}

GeometryGL::~GeometryGL() {
    deinitialize();
}

void GeometryGL::initialize() {}

void GeometryGL::deinitialize() {}

} // namespace

