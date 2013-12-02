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

#ifndef IVW_MESHGLCONVERTER_H
#define IVW_MESHGLCONVERTER_H

#include <inviwo/core/datastructures/representationconverter.h>
#include <modules/opengl/geometry/meshgl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API MeshRAM2GLConverter : public RepresentationConverterType<GeometryGL> {

public:
    MeshRAM2GLConverter();
    virtual ~MeshRAM2GLConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        // FIXME: Implement
        return NULL;//dynamic_cast<const MeshRAM*>(source) != NULL;
    }
    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

} // namespace

#endif // IVW_MESHGLCONVERTER_H
