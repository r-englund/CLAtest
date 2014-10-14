/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Contact: Martin Falk
 *
 *********************************************************************************/

#include "meshcreator.h"
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>
#include <inviwo/core/datastructures/geometry/basicmesh.h>

namespace inviwo {

ProcessorClassIdentifier(MeshCreator, "org.inviwo.MeshCreator");
ProcessorDisplayName(MeshCreator,  "Mesh Creator");
ProcessorTags(MeshCreator, Tags::None);
ProcessorCategory(MeshCreator, "Geometry Creation");
ProcessorCodeState(MeshCreator, CODE_STATE_STABLE);

MeshCreator::MeshCreator()
    : Processor(),
      outport_("outport"),
      meshScale_("scale", "Size scaling", 1.f, 0.01f, 10.f),
      meshResX_("resX", "Mesh res. (horiz.)", 16, 1, 1024),
      meshResY_("resY", "Mesh res. (vert.)", 16, 1, 1024),
      meshType_("meshType", "Mesh Type") {

    addPort(outport_);
    
    meshType_.addOption("sphere", "Sphere", SPHERE);
    meshType_.addOption("colorsphere", "Color Sphere", COLOR_SPHERE);
    meshType_.addOption("cube", "Cube", CUBE);
    meshType_.addOption("linecube", "Line cube", LINE_CUBE);
    meshType_.addOption("plane", "Plane", PLANE);
    meshType_.addOption("disk", "Disk", DISK);
    meshType_.addOption("cone", "Cone", CONE);
    meshType_.addOption("cylinder", "Cylinder", CYLINDER);
    meshType_.addOption("arrow", "Arrow", ARROW);
    meshType_.addOption("coordaxes", "Coordinate Indicator", COORD_AXES);
    
    meshType_.set(SPHERE);
    meshType_.setCurrentStateAsDefault();

    addProperty(meshType_);
    addProperty(meshScale_);
    addProperty(meshResX_);
    addProperty(meshResY_);
}

MeshCreator::~MeshCreator() {}

void MeshCreator::initialize() {
    Processor::initialize();
}

void MeshCreator::deinitialize() {
    Processor::deinitialize();
}

Mesh* MeshCreator::createMesh() {
    switch (meshType_.getSelectedIndex()) {
    case SPHERE:
        return SimpleMeshCreator::sphere(0.5f*meshScale_.get(), meshResY_.get(), meshResX_.get());
    case COLOR_SPHERE:
        // TODO: use given mesh resolution!
        return BasicMesh::colorsphere(vec3(0.0f, 0.0f, 0.0f), meshScale_.get());
    case CUBE: 
        { 
            vec3 posLLF = vec3(0.0f);
            vec3 posURB = vec3(1.0f)*meshScale_.get();
            return SimpleMeshCreator::rectangularPrism(posLLF, posURB, posLLF, posURB, vec4(posLLF, 1.f), vec4(posURB, 1.f));
        }
    case LINE_CUBE:
        return BasicMesh::boundingbox(mat4(1.0), vec4(1.0f, 0.0f, 0.0f, 1.0f));
    case PLANE:
        {
            glm::vec3 pos(0.0f);
            glm::vec2 extent(1.0f, 1.0f);
            return SimpleMeshCreator::plane(pos, extent * meshScale_.get(), meshResX_.get(), meshResY_.get());
        }
    case DISK:
        return BasicMesh::disk(vec3(0.0f, 0.0f, 0.0f), 
            vec3(0.0f, 0.0f, 1.0f), 
            vec4(1.0f,0.0f,0.0f,1.0f),
            meshScale_.get(),
            meshResX_.get());
    case CONE:
        return BasicMesh::cone(vec3(0.0f, 0.0f, -meshScale_.get()), 
            vec3(0.0f, 0.0f, meshScale_.get()), 
            vec4(1.0f, 0.0f, 0.0f, 1.0f), 
            meshScale_.get(),
            meshResX_.get());
    case CYLINDER:
        return BasicMesh::cylinder(vec3(0.0f, 0.0f, -3*meshScale_.get()),
            vec3(0.0f, 0.0f, 3*meshScale_.get()),
            vec4(1.0f, 0.0f, 0.0f, 1.0f), meshScale_.get(),
            meshResX_.get());
    case ARROW:
        return BasicMesh::arrow(vec3(0.0f, 0.0f, -3 * meshScale_.get()),
            vec3(0.0f, 0.0f, 3 * meshScale_.get()),
            vec4(1.0f, 0.0f, 0.0f, 1.0f),
            meshScale_.get(),
            0.15f,
            meshScale_.get()*2,
            meshResX_.get());
    case COORD_AXES:
        return BasicMesh::coordindicator(vec3(0.0f, 0.0f, 0.0f), meshScale_.get());
    default:
        return SimpleMeshCreator::sphere(0.1f, meshResY_.get(), meshResX_.get());
    }
}

void MeshCreator::process() {
    outport_.setData(createMesh());
}

} // namespace
