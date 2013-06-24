#include "cubeproxygeometry.h"
#include <inviwo/core/datastructures/geometry/basemeshcreator.h>

namespace inviwo {

ProcessorClassName(CubeProxyGeometry, "CubeProxyGeometry"); 
ProcessorCategory(CubeProxyGeometry, "Proxy Geometry");
ProcessorCodeState(CubeProxyGeometry, CODE_STATE_BROKEN); 

CubeProxyGeometry::CubeProxyGeometry()
    : ProcessorGL(),
      inport_("volume.inport"),
      outport_("geometry.outport"),
      clippingEnabled_("clippingEnabled", "Enable Clipping", true),
      clipX_("clipX", "Clip X Slices", 0, 256, 0, 256),
      clipY_("clipY", "Clip Y Slices", 0, 256, 0, 256),
      clipZ_("clipZ", "Clip Z Slices", 0, 256, 0, 256)
{
    addPort(inport_);
    addPort(outport_);

    addProperty(clippingEnabled_);
    addProperty(clipX_);
    addProperty(clipY_);
    addProperty(clipZ_);

    dims_ = glm::uvec3(1,1,1);
}

CubeProxyGeometry::~CubeProxyGeometry() {}

void CubeProxyGeometry::initialize() {
    ProcessorGL::initialize();
}

void CubeProxyGeometry::deinitialize() {
    ProcessorGL::deinitialize();
}

void CubeProxyGeometry::process() {
    if(dims_ != inport_.getData()->getDimension()){
        dims_ = inport_.getData()->getDimension();
        clipX_.setRangeMax(static_cast<int>(dims_.x));
        clipY_.setRangeMax(static_cast<int>(dims_.y));
        clipZ_.setRangeMax(static_cast<int>(dims_.z));
        clipX_.set(glm::ivec2(0, static_cast<int>(dims_.x)));
        clipY_.set(glm::ivec2(0, static_cast<int>(dims_.y)));
        clipZ_.set(glm::ivec2(0, static_cast<int>(dims_.z)));
    }

    glm::vec3 posLLF = glm::vec3(-1.f);
    glm::vec3 posURB = glm::vec3(1.f);
    glm::vec3 texLLF = glm::vec3(0.f);
    glm::vec3 texURB = glm::vec3(1.f);
    glm::vec4 colLLF = glm::vec4(0.f, 0.f, 0.f, 1.f);
    glm::vec4 colURB = glm::vec4(1.f);

    if(clippingEnabled_.get()){
        //Vertex positions
        posLLF.x = ((static_cast<float>(clipX_.get().x)/static_cast<float>(dims_.x))*2.f)-1.f;
        posLLF.y = ((static_cast<float>(clipY_.get().x)/static_cast<float>(dims_.y))*2.f)-1.f;
        posLLF.z = ((static_cast<float>(clipZ_.get().x)/static_cast<float>(dims_.z))*2.f)-1.f;
        posURB.x = ((static_cast<float>(clipX_.get().y)/static_cast<float>(dims_.x))*2.f)-1.f;
        posURB.y = ((static_cast<float>(clipY_.get().y)/static_cast<float>(dims_.y))*2.f)-1.f;
        posURB.z = ((static_cast<float>(clipZ_.get().y)/static_cast<float>(dims_.z))*2.f)-1.f;

        //Texture Coordinates and Colors
        texLLF.x = colLLF.x = static_cast<float>(clipX_.get().x)/static_cast<float>(dims_.x);
        texLLF.y = colLLF.y = static_cast<float>(clipY_.get().x)/static_cast<float>(dims_.y);
        texLLF.z = colLLF.z = static_cast<float>(clipZ_.get().x)/static_cast<float>(dims_.z);
        texURB.x = colURB.x = static_cast<float>(clipX_.get().y)/static_cast<float>(dims_.x);
        texURB.y = colURB.y = static_cast<float>(clipY_.get().y)/static_cast<float>(dims_.y);
        texURB.z = colURB.z = static_cast<float>(clipZ_.get().y)/static_cast<float>(dims_.z);
    }

    //Create Rectangular Prism and set it to the outport
    outport_.setData(new Geometry(BaseMeshCreator::rectangularPrism(posLLF, posURB, texLLF, texURB, colLLF, colURB)));
}

} // namespace
