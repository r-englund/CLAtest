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

#ifndef IVW_MESH_H
#define IVW_MESH_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/attributes.h>
#include <inviwo/core/datastructures/geometry/geometry.h>
#include <inviwo/core/datastructures/geometry/geometrytype.h>
#include <utility>

namespace inviwo {

class IVW_CORE_API Mesh : public Geometry {

public:
    struct AttributesInfo {
        RenderType rt;
        ConnectivityType ct;
        AttributesInfo() : rt(POINTS), ct(NONE){}
        AttributesInfo(RenderType r, ConnectivityType c) : rt(r), ct(c){}
    };

    Mesh();
    Mesh(const Mesh& rhs);
    Mesh(RenderType rt, ConnectivityType ct);
    virtual ~Mesh();
    virtual void performOperation(DataOperation*) const {};
    virtual void initialize();
    virtual void deinitialize();
    virtual Mesh* clone() const;

    void addAttribute(Buffer*);
    void addIndicies(AttributesInfo info, IndexBuffer* ind);

    const std::vector<Buffer*>& getBuffers() { return attributes_; }
    const std::vector<std::pair<AttributesInfo, IndexBuffer*> >& getIndexBuffers() { return indexAttributes_; }

    const Buffer* getAttributes(size_t idx) const;
    const Buffer* getIndicies(size_t idx) const;

    Buffer* getAttributes(size_t idx);
    Buffer* getIndicies(size_t idx);

    AttributesInfo getAttributesInfo() const;
    AttributesInfo getIndexAttributesInfo(size_t idx) const;

    size_t getNumberOfAttributes() const;
    size_t getNumberOfIndicies() const;

protected:
    std::vector<Buffer*> attributes_;
    AttributesInfo attributesInfo_;
    std::vector<std::pair<AttributesInfo, IndexBuffer*> > indexAttributes_;

};


} // namespace

#endif // IVW_MESH_H
