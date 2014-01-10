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
#include <modules/opengl/buffer/elementbuffergl.h>
#include <modules/opengl/geometry/meshgl.h>
#include <modules/opengl/geometry/attributebuffergl.h>

namespace inviwo {

MeshGL::MeshGL()
    : GeometryGL(){
}

MeshGL::MeshGL(const MeshGL& rhs) 
: GeometryGL(rhs){
}

MeshGL::~MeshGL() {
    deinitialize();
}

void MeshGL::initialize() {}

void MeshGL::deinitialize() {}

DataRepresentation* MeshGL::clone() const{
    return new MeshGL(*this);
}

void MeshGL::enable() const{
    for (std::vector<const BufferGL*>::const_iterator it = attributesGL_.begin(); it != attributesGL_.end(); ++it) {
        (*it)->enable();
    }
    glPushMatrix();
    mat4 modelToWorld = owner_->getWorldTransform();
    glMultMatrixf(glm::value_ptr(modelToWorld));
    mat4 dataToModel = owner_->getBasisAndOffset();
    glMultMatrixf(glm::value_ptr(dataToModel));
}

void MeshGL::disable() const{
    glPopMatrix();
    for (std::vector<const BufferGL*>::const_iterator it = attributesGL_.begin(); it != attributesGL_.end(); ++it) {
        (*it)->disable();
    }
}

void MeshGL::update(bool editable){
    attributesGL_.clear();
    if(editable){
        for (std::vector<Buffer*>::const_iterator it = owner_->getBuffers().begin(); it != owner_->getBuffers().end(); ++it) {
            attributesGL_.push_back((*it)->getEditableRepresentation<BufferGL>());
        }
    }
    else{
        for (std::vector<Buffer*>::const_iterator it = owner_->getBuffers().begin(); it != owner_->getBuffers().end(); ++it) {
            attributesGL_.push_back((*it)->getRepresentation<BufferGL>());
        }
    }
}

void MeshGL::setPointerToOwner(DataGroup* owner){
    owner_ = dynamic_cast<Mesh*>(owner);
}


} // namespace

