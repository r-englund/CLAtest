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

#include <inviwo/core/datastructures/geometry/mesh.h>

namespace inviwo {

Mesh::Mesh() : Geometry(), attributesInfo_(AttributesInfo()) {}

Mesh::Mesh(const Mesh& rhs) : Geometry(rhs), attributesInfo_(rhs.attributesInfo_) {
    std::vector<bool>::const_iterator itOwnership = rhs.attributesOwnership_.begin();
    for (std::vector<Buffer*>::const_iterator it = rhs.attributes_.begin(), itEnd = rhs.attributes_.end(); it != itEnd; ++it, ++itOwnership) {
        if (*itOwnership) {
            addAttribute(static_cast<Buffer*>((*it)->clone()));
        } else {
            addAttribute(*it, false);
        }
    }

    for (std::vector<std::pair<AttributesInfo, IndexBuffer*> >::const_iterator it = rhs.indexAttributes_.begin() ;
         it != rhs.indexAttributes_.end(); ++it)
        addIndicies(it->first, static_cast<IndexBuffer*>(it->second->clone()));
}

Mesh::Mesh(GeometryEnums::RenderType rt, GeometryEnums::ConnectivityType ct)
    : Geometry(), attributesInfo_(AttributesInfo(rt, ct))
{}

Mesh::~Mesh() {
    deinitialize();
}

std::string Mesh::getDataInfo() const{
    std::ostringstream stream;
    stream << "Type: mesh\n";

    stream << "Data: ";
    switch(getAttributesInfo().rt){
        case GeometryEnums::POINTS:
            stream << "Points";
            break;
        case GeometryEnums::LINES:
            stream << "Lines";
            break;
        case GeometryEnums::TRIANGLES:
            stream << "Triangles";
            break;
        default:
            stream << "Not specified"; 
    }
    stream << "\n";
    
    return stream.str();
}

Mesh* Mesh::clone() const {
    return new Mesh(*this);
}

void Mesh::initialize() {}

void Mesh::deinitialize() {
    std::vector<bool>::const_iterator itOwnership = attributesOwnership_.begin();
    for (std::vector<Buffer*>::iterator it = attributes_.begin(), itEnd = attributes_.end(); it != itEnd; ++it, ++itOwnership) {
        if (*itOwnership)
            delete(*it);
    }

    for (std::vector<std::pair<AttributesInfo, IndexBuffer*> >::iterator it = indexAttributes_.begin(), itEnd = indexAttributes_.end(); it != itEnd; ++it)
        delete it->second;
}

void Mesh::addAttribute(Buffer* att, bool takeOwnership /*= true*/) {
    attributes_.push_back(att);
    attributesOwnership_.push_back(takeOwnership);
}


void Mesh::setAttribute(size_t idx, Buffer* att, bool takeOwnership /*= true*/) {
    if (idx < attributes_.size()) {
        if (attributesOwnership_[idx]) {
            delete attributes_[idx];
        }
        attributes_[idx] = att;
        attributesOwnership_[idx] = takeOwnership;
    }
}

void Mesh::addIndicies(AttributesInfo info, IndexBuffer* ind) {
    indexAttributes_.push_back(std::make_pair(info, ind));
}

const Buffer* Mesh::getAttributes(size_t idx) const {
    return attributes_[idx];
}

const Buffer* Mesh::getIndicies(size_t idx) const {
    return indexAttributes_[idx].second;
}

Buffer* Mesh::getAttributes(size_t idx) {
    return attributes_[idx];
}

Buffer* Mesh::getIndicies(size_t idx) {
    return indexAttributes_[idx].second;
}

Mesh::AttributesInfo Mesh::getAttributesInfo() const {
    return attributesInfo_;
}

Mesh::AttributesInfo Mesh::getIndexAttributesInfo(size_t idx) const {
    return indexAttributes_[idx].first;
}

size_t Mesh::getNumberOfAttributes() const {
    return attributes_.size();
}

size_t Mesh::getNumberOfIndicies() const {
    return indexAttributes_.size();
}

} // namespace

