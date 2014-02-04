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
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

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
