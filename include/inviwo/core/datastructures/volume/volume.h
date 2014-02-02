/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#ifndef IVW_VOLUME_H
#define IVW_VOLUME_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/spatialdata.h>

namespace inviwo {

class VolumeRepresentation;

class IVW_CORE_API Volume : public Data, public StructuredGridEntity<3> {
public:
    Volume(uvec3 dimensions = uvec3(128,128,128), const DataFormatBase* format = DataUINT8::get());
    Volume(const Volume&);
    Volume(VolumeRepresentation*);
    Volume& operator=(const Volume& that);
    virtual Volume* clone() const;
    virtual ~Volume();


    uvec3 getDimension() const;
    void setDimension(const uvec3& dim);

    void setOffset(const vec3& offset);
    vec3 getOffset() const;

    mat3 getBasis() const;
    void setBasis(const mat3& basis);

    mat4 getBasisAndOffset() const;
    void setBasisAndOffset(const mat4& mat);

    mat4 getWorldTransform() const;
    void setWorldTransform(const mat4& mat);

protected:
    virtual DataRepresentation* createDefaultRepresentation();
};

} // namespace

#endif // IVW_VOLUME_H
