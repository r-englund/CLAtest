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

#include <inviwo/core/datastructures/geometry/mesh.h>

namespace inviwo {

Mesh::Mesh() : Geometry(), attributesInfo_(AttributesInfo()){}

Mesh::Mesh(const Mesh& rhs) : Geometry(rhs), attributesInfo_(rhs.attributesInfo_) {
    for(std::vector<Buffer*>::const_iterator it = rhs.attributes_.begin() ; it != rhs.attributes_.end(); ++it)
       addAttribute(static_cast<Buffer*>((*it)->clone()));

    for(std::vector<std::pair<AttributesInfo, IndexBuffer*> >::const_iterator it = rhs.indexAttributes_.begin() ; it != rhs.indexAttributes_.end(); ++it)
       addIndicies(it->first, static_cast<IndexBuffer*>(it->second->clone()));
}

Mesh::Mesh(RenderType rt, ConnectivityType ct)
    : Geometry(), attributesInfo_(AttributesInfo(rt, ct))
{}

Mesh::~Mesh() {
    deinitialize();
}

Mesh* Mesh::clone() const {
    return new Mesh(*this);
}

void Mesh::initialize() {}

void Mesh::deinitialize() {
    for (std::vector<Buffer*>::iterator it = attributes_.begin() ; it != attributes_.end(); ++it)
        delete (*it);

    for (std::vector<std::pair<AttributesInfo, IndexBuffer*> >::iterator it = indexAttributes_.begin() ; it != indexAttributes_.end(); ++it)
        delete it->second;
}

void Mesh::addAttribute(Buffer* att){
    attributes_.push_back(att);
}

void Mesh::addIndicies(AttributesInfo info, IndexBuffer* ind){
    indexAttributes_.push_back(std::make_pair(info, ind));
}

Buffer* Mesh::getAttributes(size_t idx) const{
    return attributes_[idx];
}

Buffer* Mesh::getIndicies(size_t idx) const{
    return indexAttributes_[idx].second;
}

Mesh::AttributesInfo Mesh::getAttributesInfo() const{
    return attributesInfo_;
}

Mesh::AttributesInfo Mesh::getIndexAttributesInfo(size_t idx) const{
    return indexAttributes_[idx].first;
}

size_t Mesh::getNumberOfAttributes() const{
    return attributes_.size();
}

size_t Mesh::getNumberOfIndicies() const{
    return indexAttributes_.size();
}

} // namespace

