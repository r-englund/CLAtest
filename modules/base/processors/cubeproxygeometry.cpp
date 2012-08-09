#include "cubeproxygeometry.h"

namespace inviwo {

CubeProxygeometry::CubeProxygeometry()
    : ProcessorGL(),
      inport_(Port::INPORT, "image.inport")
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
    std::cout << "CubeProxygeometry::process()" << std::endl;
}

} // namespace
