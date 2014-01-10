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
#include <inviwo/core/datastructures/geometry/meshram.h>
#include <inviwo/core/datastructures/buffer/bufferram.h>

namespace inviwo {

MeshRAM::MeshRAM()
    : GeometryRAM(){
}

MeshRAM::MeshRAM(const MeshRAM& rhs) 
: GeometryRAM(rhs), owner_(rhs.owner_){
    update(true);
}

MeshRAM::~MeshRAM() {
    deinitialize();
}

void MeshRAM::initialize() {}

void MeshRAM::deinitialize() {}

DataRepresentation* MeshRAM::clone() const{
    return new MeshRAM(*this);
}

void MeshRAM::update(bool editable){
    attributesRAM_.clear();
    if(editable){
        for (std::vector<Buffer*>::const_iterator it = owner_->getBuffers().begin(); it != owner_->getBuffers().end(); ++it) {
            attributesRAM_.push_back((*it)->getEditableRepresentation<BufferRAM>());
        }
    }
    else{
        for (std::vector<Buffer*>::const_iterator it = owner_->getBuffers().begin(); it != owner_->getBuffers().end(); ++it) {
            attributesRAM_.push_back(const_cast<BufferRAM*>((*it)->getRepresentation<BufferRAM>()));
        }
    }
}

void MeshRAM::setPointerToOwner(DataGroup* owner){
    owner_ = dynamic_cast<Mesh*>(owner);
}


} // namespace

