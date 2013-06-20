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
      testI_("testInt", "Min Max Test", 4, 7),
      testF_("testFloat", "Min Max Test", 0.0f, 1.0),
      clipX_("clipX", "Clip X Axis", glm::vec2(0.0f, 1.0f)),
      clipY_("clipY", "Clip Y Axis", glm::vec2(0.0f, 1.0f)),
      clipZ_("clipZ", "Clip Z Axis", glm::vec2(0.0f, 1.0f))
{
    addPort(inport_);
    addPort(outport_);

    addProperty(clippingEnabled_);
    addProperty(testI_);
    addProperty(testF_);
    addProperty(clipX_);
    addProperty(clipY_);
    addProperty(clipZ_);
}

CubeProxyGeometry::~CubeProxyGeometry() {}

void CubeProxyGeometry::initialize() {
    ProcessorGL::initialize();
}

void CubeProxyGeometry::deinitialize() {
    ProcessorGL::deinitialize();
}

void CubeProxyGeometry::process() {
    glm::vec3 posLLF = glm::vec3(-1.f);
    glm::vec3 posURB = glm::vec3(1.f);
    glm::vec3 texLLF = glm::vec3(0.f);
    glm::vec3 texURB = glm::vec3(1.f);
    glm::vec4 colLLF = glm::vec4(0.f, 0.f, 0.f, 1.f);
    glm::vec4 colURB = glm::vec4(1.f);

    if(clippingEnabled_.get()){
        //Vertex positions
        posLLF.x = (clipX_.get().x*2.f)-1.f;
        posLLF.y = (clipY_.get().x*2.f)-1.f;
        posLLF.z = (clipZ_.get().x*2.f)-1.f;
        posURB.x = (clipX_.get().y*2.f)-1.f;
        posURB.y = (clipY_.get().y*2.f)-1.f;
        posURB.z = (clipZ_.get().y*2.f)-1.f;

        //Texture Coordinates and Colors
        texLLF.x = colLLF.x = clipX_.get().x;
        texLLF.y = colLLF.y = clipY_.get().x;
        texLLF.z = colLLF.z = clipZ_.get().x;
        texURB.x = colURB.x = clipX_.get().y;
        texURB.y = colURB.y = clipY_.get().y;
        texURB.z = colURB.z = clipZ_.get().y;
    }

    //Create Rectangular Prism and set it to the outport
    outport_.setData(new Geometry(BaseMeshCreator::rectangularPrism(posLLF, posURB, texLLF, texURB, colLLF, colURB)));
}

} // namespace
