/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file authors: Erik Sundén, Daniel Jönsson
 *
 *********************************************************************************/

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
        AttributesInfo() : rt(POINTS), ct(NONE) {}
        AttributesInfo(RenderType r, ConnectivityType c) : rt(r), ct(c) {}
    };

    Mesh();
    Mesh(const Mesh& rhs);
    Mesh(RenderType rt, ConnectivityType ct);
    virtual ~Mesh();
    virtual std::string getDataInfo() const;

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
