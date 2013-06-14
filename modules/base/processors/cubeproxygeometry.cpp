#include "cubeproxygeometry.h"
#include <inviwo/core/datastructures/geometry/basemeshcreator.h>

namespace inviwo {

ProcessorClassName(CubeProxyGeometry, "CubeProxyGeometry"); 
ProcessorCategory(CubeProxyGeometry, "Proxy Geometry");
ProcessorCodeState(CubeProxyGeometry, CODE_STATE_BROKEN); 

CubeProxyGeometry::CubeProxyGeometry()
    : ProcessorGL(),
      inport_("volume.inport"),
      outport_("geometry.outport")
{
    addPort(inport_);
    addPort(outport_);
}

CubeProxyGeometry::~CubeProxyGeometry() {}

void CubeProxyGeometry::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("img_identity.frag");
}

void CubeProxyGeometry::deinitialize() {
    delete shader_;
    ProcessorGL::deinitialize();
}

void CubeProxyGeometry::process() {
    BaseMeshRAM* prism = BaseMeshCreator::rectangularPrism(glm::vec3(-1.f), glm::vec3(1.f), glm::vec3(0.f), glm::vec3(1.f), glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec4(1.f));
    outport_.getData()->addRepresentation(prism);
}

} // namespace
