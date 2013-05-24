#include "cubeproxygeometry.h"

namespace inviwo {

ProcessorClassName(CubeProxygeometry, "CubeProxygeometry"); 
ProcessorCategory(CubeProxygeometry, "Proxy Geometry");
ProcessorCodeState(CubeProxygeometry, CODE_STATE_BROKEN); 

CubeProxygeometry::CubeProxygeometry()
    : ProcessorGL(),
      inport_("image.inport")
{
    addPort(inport_);
}

CubeProxygeometry::~CubeProxygeometry() {}

void CubeProxygeometry::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("img_identity.frag");
}

void CubeProxygeometry::deinitialize() {
    delete shader_;
    ProcessorGL::deinitialize();
}

void CubeProxygeometry::process() {
}

} // namespace
