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

#ifndef IVW_GEOMETRYTYPE_H
#define IVW_GEOMETRYTYPE_H

namespace inviwo {

enum CoordinatePlane {
    XY,
    XZ,
    YZ
};

enum RenderType {
    NOT_SPECIFIED,
    POINTS,
    LINES,
    TRIANGLES,
    NUMBER_OF_RENDER_TYPES
};

enum ConnectivityType {
    NONE,
    STRIP,
    LOOP,
    FAN,
    ADJACENCY,
    STRIP_ADJACENCY,
    NUMBER_OF_CONNECTIVITY_TYPES
};

} // namespace

#endif // IVW_GEOMETRYTYPE_H
