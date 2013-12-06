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

#include "meshcreator.h"
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>

namespace inviwo {

ProcessorClassName(MeshCreator, "MeshCreator"); 
ProcessorCategory(MeshCreator, "Geometry Creation");
ProcessorCodeState(MeshCreator, CODE_STATE_STABLE); 

MeshCreator::MeshCreator()
    : Processor(),
      outport_("outport"),
      meshScale_("scale", "Size scaling", 1.f, 0.01f, 10.f),
      meshType_("meshType", "Mesh Type")
{
    addPort(outport_);

    meshType_.addOption("cube", "Cube");
    meshType_.addOption("sphere", "Sphere");
    meshType_.set("sphere");
    addProperty(meshType_);
    addProperty(meshScale_);
}

MeshCreator::~MeshCreator() {}

void MeshCreator::initialize() {
    Processor::initialize();
}

void MeshCreator::deinitialize() {
    Processor::deinitialize();
}

SimpleMesh* MeshCreator::createMesh(){
    if(meshType_.get() == "sphere"){
        return SimpleMeshCreator::sphere(0.5f*meshScale_.get(), 8, 16);
    }
    else{
        vec3 posLLF = vec3(0.0f);
        vec3 posURB = vec3(1.0f)*meshScale_.get();
        return SimpleMeshCreator::rectangularPrism(posLLF, posURB, posLLF, posURB, vec4(posLLF, 1.f), vec4(posURB, 1.f));
    }
}

void MeshCreator::process() {  
    outport_.setData(createMesh());
}

} // namespace
