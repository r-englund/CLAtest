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

#ifndef IVW_MESHRAM_H
#define IVW_MESHRAM_H

#include <inviwo/core/datastructures/geometry/geometryram.h>

namespace inviwo {

class Mesh;
class BufferRAM;

class IVW_CORE_API MeshRAM : public GeometryRAM {

public:
    MeshRAM();
    MeshRAM(const MeshRAM& rhs);
    virtual ~MeshRAM();
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;

protected:
    virtual void update(bool editable);

    virtual void setPointerToOwner(DataGroup*);

    Mesh* owner_;

    std::vector<BufferRAM*> attributesRAM_;
};

} // namespace

#endif // IVW_MESHRAM_H
