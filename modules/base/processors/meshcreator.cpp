#include "meshcreator.h"
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>

namespace inviwo {

ProcessorClassName(MeshCreator, "MeshCreator"); 
ProcessorCategory(MeshCreator, "Geometry Creation");
ProcessorCodeState(MeshCreator, CODE_STATE_STABLE); 

MeshCreator::MeshCreator()
    : Processor(),
      outport_("outport"),
      meshType_("meshType", "Mesh Type")
{
    addPort(outport_);

    meshType_.addOption("cube", "Cube");
    meshType_.addOption("sphere", "Sphere");
    meshType_.set("sphere");
    addProperty(meshType_);
}

MeshCreator::~MeshCreator() {}

void MeshCreator::initialize() {
    Processor::initialize();
}

void MeshCreator::deinitialize() {
    Processor::deinitialize();
}

SimpleMeshRAM* MeshCreator::createMesh(){
    if(meshType_.get() == "sphere"){
        return SimpleMeshCreator::sphere(0.5f, 8, 16);
    }
    else{
        vec3 posLLF = vec3(0.0f);
        vec3 posURB = vec3(1.0f);
        return SimpleMeshCreator::rectangularPrism(posLLF, posURB, posLLF, posURB, vec4(posLLF, 1.f), vec4(posURB, 1.f));
    }
}

void MeshCreator::process() {  
    outport_.setData(createMesh());
}

} // namespace
