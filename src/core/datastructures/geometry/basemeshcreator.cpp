#include <inviwo/core/datastructures/geometry/basemeshcreator.h>

namespace inviwo {

TriangleMeshRAM* BaseMeshCreator::rectangularPrism(glm::vec3 posLlf, glm::vec3 posUrb, glm::vec3 texCoordLlf, 
                                                   glm::vec3 texCoordUrb, glm::vec4 colorLlf, glm::vec4 colorUrb) {
    TriangleMeshRAM* recPrism = new TriangleMeshRAM(GeometryRepresentation::STRIP);
    recPrism->initialize();

    //8 corners
    unsigned int LLF = recPrism->addVertex(posLlf, texCoordLlf, colorLlf);
    unsigned int LRF = recPrism->addVertex(glm::vec3(posLlf.x, posUrb.y, posLlf.z), glm::vec3(texCoordLlf.x, texCoordUrb.y, texCoordLlf.z), glm::vec4(colorLlf.x, colorUrb.y, colorLlf.z, colorLlf.w));
    unsigned int LRB = recPrism->addVertex(glm::vec3(posLlf.x, posUrb.y, posUrb.z), glm::vec3(texCoordLlf.x, texCoordUrb.y, texCoordUrb.z), glm::vec4(colorLlf.x, colorUrb.y, colorUrb.z, colorLlf.w));
    unsigned int LLB = recPrism->addVertex(glm::vec3(posLlf.x, posLlf.y, posUrb.z), glm::vec3(texCoordLlf.x, texCoordLlf.y, texCoordUrb.z), glm::vec4(colorLlf.x, colorLlf.y, colorUrb.z, colorLlf.w));
    unsigned int ULB = recPrism->addVertex(glm::vec3(posUrb.x, posLlf.y, posUrb.z), glm::vec3(texCoordUrb.x, texCoordLlf.y, texCoordUrb.z), glm::vec4(colorUrb.x, colorLlf.y, colorUrb.z, colorUrb.w));
    unsigned int ULF = recPrism->addVertex(glm::vec3(posUrb.x, posLlf.y, posLlf.z), glm::vec3(texCoordUrb.x, texCoordLlf.y, texCoordLlf.z), glm::vec4(colorUrb.x, colorLlf.y, colorLlf.z, colorUrb.w));
    unsigned int URF = recPrism->addVertex(glm::vec3(posUrb.x, posUrb.y, posLlf.z), glm::vec3(texCoordUrb.x, texCoordUrb.y, texCoordLlf.z), glm::vec4(colorUrb.x, colorUrb.y, colorLlf.z, colorUrb.w));
    unsigned int URB = recPrism->addVertex(posUrb, texCoordUrb, colorUrb);

    //14 indices (Triangle Strip)
    recPrism->addIndex(LLB);
    recPrism->addIndex(ULB);
    recPrism->addIndex(LRB);
    recPrism->addIndex(URB);
    recPrism->addIndex(URF);
    recPrism->addIndex(ULB);
    recPrism->addIndex(ULF);
    recPrism->addIndex(LLF);
    recPrism->addIndex(URF);
    recPrism->addIndex(LRF);
    recPrism->addIndex(LRB);
    recPrism->addIndex(LLF);
    recPrism->addIndex(LLB);
    recPrism->addIndex(ULB);

    return recPrism;
}

} // namespace

