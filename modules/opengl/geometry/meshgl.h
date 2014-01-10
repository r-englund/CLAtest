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

#ifndef IVW_MESHGL_H
#define IVW_MESHGL_H

#include <modules/opengl/geometry/geometrygl.h>

namespace inviwo {

class Mesh;
class BufferGL;

class IVW_MODULE_OPENGL_API MeshGL : public GeometryGL {

public:
    MeshGL();
    MeshGL(const MeshGL& rhs);
    virtual ~MeshGL();
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const;

    void enable() const;
    void disable() const;

protected:
    virtual void update(bool editable);

    virtual void setPointerToOwner(DataGroup*);

private:
    Mesh* owner_;

    std::vector<const BufferGL*> attributesGL_;

};

} // namespace

#endif // IVW_MESHGL_H
