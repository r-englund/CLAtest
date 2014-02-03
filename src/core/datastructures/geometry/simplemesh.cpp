 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013 Inviwo Foundation
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

#include <inviwo/core/datastructures/geometry/simplemesh.h>

namespace inviwo {

SimpleMesh::SimpleMesh(RenderType rt, ConnectivityType ct)
    : Mesh(rt, ct)
{
}

SimpleMesh::~SimpleMesh() {
    deinitialize();
}

void SimpleMesh::initialize() {
    vertexPositions_ = new Position3dBuffer();
    addAttribute(vertexPositions_);
    vertexTexCoords_ = new TexCoord3dBuffer();
    addAttribute(vertexTexCoords_);
    vertexColors_ = new ColorBuffer();
    addAttribute(vertexColors_);
    indices_ = new IndexBuffer();
    addIndicies(Mesh::AttributesInfo(), indices_);
}

void SimpleMesh::deinitialize() {}

void SimpleMesh::addVertex(vec3 pos, vec3 texCoord, vec4 color) {
    vertexPositions_->getEditableRepresentation<Position3dBufferRAM>()->add(pos);
    vertexTexCoords_->getEditableRepresentation<TexCoord3dBufferRAM>()->add(texCoord);
    vertexColors_->getEditableRepresentation<ColorBufferRAM>()->add(color);
}

void SimpleMesh::addIndex(unsigned int idx) {
    indices_->getEditableRepresentation<IndexBufferRAM>()->add(idx);
}

void SimpleMesh::setIndicesInfo(RenderType rt, ConnectivityType ct) {
    indexAttributes_[0].first = Mesh::AttributesInfo(rt, ct);
}

const Position3dBuffer* SimpleMesh::getVertexList() const {
    return this->vertexPositions_;
}

const TexCoord3dBuffer* SimpleMesh::getTexCoordList() const {
    return this->vertexTexCoords_;
}

const ColorBuffer* SimpleMesh::getColorList() const {
    return this->vertexColors_;
}

const IndexBuffer* SimpleMesh::getIndexList() const {
    return this->indices_;
}

} // namespace

