#include <inviwo/core/datastructures/geometry/basemeshcreator.h>

namespace inviwo {

BaseMeshRAM* BaseMeshCreator::rectangularPrism(glm::vec3 posLlf, glm::vec3 posUrb, glm::vec3 texCoordLlf, 
                                               glm::vec3 texCoordUrb, glm::vec4 colorLlf, glm::vec4 colorUrb) {
    BaseMeshRAM* recPrism = new BaseMeshRAM();
    recPrism->initialize();

    //8 corners
    recPrism->addVertex(posLlf, texCoordLlf, colorLlf);
    recPrism->addVertex(glm::vec3(posLlf.x, posUrb.y, posLlf.z), glm::vec3(texCoordLlf.x, texCoordUrb.y, texCoordLlf.z), glm::vec4(colorLlf.x, colorUrb.y, colorLlf.z, colorLlf.w));
    recPrism->addVertex(glm::vec3(posLlf.x, posUrb.y, posUrb.z), glm::vec3(texCoordLlf.x, texCoordUrb.y, texCoordUrb.z), glm::vec4(colorLlf.x, colorUrb.y, colorUrb.z, colorLlf.w));
    recPrism->addVertex(glm::vec3(posLlf.x, posLlf.y, posUrb.z), glm::vec3(texCoordLlf.x, texCoordLlf.y, texCoordUrb.z), glm::vec4(colorLlf.x, colorLlf.y, colorUrb.z, colorLlf.w));
    recPrism->addVertex(glm::vec3(posUrb.x, posLlf.y, posUrb.z), glm::vec3(texCoordUrb.x, texCoordLlf.y, texCoordUrb.z), glm::vec4(colorUrb.x, colorLlf.y, colorUrb.z, colorUrb.w));
    recPrism->addVertex(glm::vec3(posUrb.x, posLlf.y, posLlf.z), glm::vec3(texCoordUrb.x, texCoordLlf.y, texCoordLlf.z), glm::vec4(colorUrb.x, colorLlf.y, colorLlf.z, colorUrb.w));
    recPrism->addVertex(glm::vec3(posUrb.x, posUrb.y, posLlf.z), glm::vec3(texCoordUrb.x, texCoordUrb.y, texCoordLlf.z), glm::vec4(colorUrb.x, colorUrb.y, colorLlf.z, colorUrb.w));
    recPrism->addVertex(posUrb, texCoordUrb, colorUrb);

    //14 indices (Triangle Strip)
    recPrism->setIndicesInfo(GeometryRepresentation::TRIANGLES, GeometryRepresentation::STRIP);
    recPrism->addIndex(3);
    recPrism->addIndex(4);
    recPrism->addIndex(2);
    recPrism->addIndex(7);
    recPrism->addIndex(6);
    recPrism->addIndex(4);
    recPrism->addIndex(5);
    recPrism->addIndex(0);
    recPrism->addIndex(6);
    recPrism->addIndex(1);
    recPrism->addIndex(2);
    recPrism->addIndex(0);
    recPrism->addIndex(3);
    recPrism->addIndex(4);

    return recPrism;
}

BaseMeshRAM* BaseMeshCreator::rectangle(glm::vec3 posLl, glm::vec3 posUr) {
	BaseMeshRAM* rec = new BaseMeshRAM();
	rec->initialize();

	glm::vec3 texCoordLl(0,0,0);
	glm::vec3 texCoordUr(1,1,0);

	glm::vec4 colorLl(1,1,1,1);
	glm::vec4 colorUr(0,1,0,1);

	// 4 corners
	rec->addVertex(posLl, texCoordLl, colorLl);
    rec->addVertex(glm::vec3(posLl.x, posUr.y, posLl.z), glm::vec3(texCoordLl.x, texCoordUr.y, texCoordLl.z), glm::vec4(colorLl.x, colorUr.y, colorLl.z, colorLl.w));
    rec->addVertex(glm::vec3(posUr.x, posLl.y, posUr.z), glm::vec3(texCoordLl.x, texCoordUr.y, texCoordUr.z), glm::vec4(colorLl.x, colorUr.y, colorUr.z, colorLl.w));
    rec->addVertex(posUr, texCoordUr, colorUr);

	// 4 indices (?)
	rec->setIndicesInfo(GeometryRepresentation::TRIANGLES, GeometryRepresentation::STRIP);
	rec->addIndex(1);
	rec->addIndex(3);
	rec->addIndex(0);
	rec->addIndex(2);

	return rec;
}

} // namespace

