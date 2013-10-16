#include "cubeproxygeometry.h"
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>

namespace inviwo {

ProcessorClassName(CubeProxyGeometry, "CubeProxyGeometry"); 
ProcessorCategory(CubeProxyGeometry, "Geometry Creation");
ProcessorCodeState(CubeProxyGeometry, CODE_STATE_STABLE); 

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

    dims_ = uvec3(1,1,1);
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
    }

    vec3 posLLF = vec3(-1.f);
    vec3 posURB = vec3(1.f);
    vec3 texLLF = vec3(0.f);
    vec3 texURB = vec3(1.f);
    vec4 colLLF = vec4(0.f, 0.f, 0.f, 1.f);
    vec4 colURB = vec4(1.f);

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
    outport_.setData(new Geometry(SimpleMeshCreator::rectangularPrism(posLLF, posURB, texLLF, texURB, colLLF, colURB)));
}

} // namespace
