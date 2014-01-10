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

#ifndef IVW_GEOMETRYRAM_H
#define IVW_GEOMETRYRAM_H

#include <inviwo/core/datastructures/geometry/geometryrepresentation.h>

namespace inviwo {

class IVW_CORE_API GeometryRAM : public GeometryRepresentation {

public:
    GeometryRAM();
    GeometryRAM(const GeometryRAM& rhs);
    virtual ~GeometryRAM();
    virtual void initialize();
    virtual void deinitialize();
};

} // namespace

#endif // IVW_GEOMETRYRAM_H
