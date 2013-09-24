#include <inviwo/core/datastructures/geometry/basemeshcreator.h>

namespace inviwo {

BaseMeshRAM* BaseMeshCreator::rectangularPrism(vec3 posLlf, vec3 posUrb, vec3 texCoordLlf, 
                                               vec3 texCoordUrb, vec4 colorLlf, vec4 colorUrb) {
    BaseMeshRAM* recPrism = new BaseMeshRAM();
    recPrism->initialize();

    //8 corners
    recPrism->addVertex(posLlf, texCoordLlf, colorLlf);
    recPrism->addVertex(vec3(posLlf.x, posUrb.y, posLlf.z), vec3(texCoordLlf.x, texCoordUrb.y, texCoordLlf.z), vec4(colorLlf.x, colorUrb.y, colorLlf.z, colorLlf.w));
    recPrism->addVertex(vec3(posLlf.x, posUrb.y, posUrb.z), vec3(texCoordLlf.x, texCoordUrb.y, texCoordUrb.z), vec4(colorLlf.x, colorUrb.y, colorUrb.z, colorLlf.w));
    recPrism->addVertex(vec3(posLlf.x, posLlf.y, posUrb.z), vec3(texCoordLlf.x, texCoordLlf.y, texCoordUrb.z), vec4(colorLlf.x, colorLlf.y, colorUrb.z, colorLlf.w));
    recPrism->addVertex(vec3(posUrb.x, posLlf.y, posUrb.z), vec3(texCoordUrb.x, texCoordLlf.y, texCoordUrb.z), vec4(colorUrb.x, colorLlf.y, colorUrb.z, colorUrb.w));
    recPrism->addVertex(vec3(posUrb.x, posLlf.y, posLlf.z), vec3(texCoordUrb.x, texCoordLlf.y, texCoordLlf.z), vec4(colorUrb.x, colorLlf.y, colorLlf.z, colorUrb.w));
    recPrism->addVertex(vec3(posUrb.x, posUrb.y, posLlf.z), vec3(texCoordUrb.x, texCoordUrb.y, texCoordLlf.z), vec4(colorUrb.x, colorUrb.y, colorLlf.z, colorUrb.w));
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

BaseMeshRAM* BaseMeshCreator::parallelepiped(glm::vec3 pos, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, 	
											 glm::vec3 tex, glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, 	
											 glm::vec4 col, glm::vec4 c1, glm::vec4 c2, glm::vec4 c3){
   
	BaseMeshRAM* ppd = new BaseMeshRAM();
    ppd->initialize();

	//8 corners
	ppd->addVertex(pos               , tex               , col               ); // (0,0,0)
	ppd->addVertex(pos      + p2     , tex      + t2     , col      + c2     ); // (0,1,0)
	ppd->addVertex(pos      + p2 + p3, tex      + t2 + t3, col      + c2 + c3); // (0,1,1)
	ppd->addVertex(pos           + p3, tex           + t3, col           + c3); // (0,0,1)
	ppd->addVertex(pos + p1      + p3, tex + t1      + t3, col + c1      + c3); // (1,0,1)
	ppd->addVertex(pos + p1          , tex + t1          , col + c1          ); // (1,0,0)
	ppd->addVertex(pos + p1 + p2     , tex + t1 + t2     , col + c1 + c2     ); // (1,1,0)
	ppd->addVertex(pos + p1 + p2 + p3, tex + t1 + t2 + t3, col + c1 + c2 + c3); // (1,1,1)

    //14 indices (Triangle Strip)
    ppd->setIndicesInfo(GeometryRepresentation::TRIANGLES, GeometryRepresentation::STRIP);
    ppd->addIndex(3);
    ppd->addIndex(4);
    ppd->addIndex(2);
    ppd->addIndex(7);
    ppd->addIndex(6);
    ppd->addIndex(4);
    ppd->addIndex(5);
    ppd->addIndex(0);
    ppd->addIndex(6);
    ppd->addIndex(1);
    ppd->addIndex(2);
    ppd->addIndex(0);
    ppd->addIndex(3);
    ppd->addIndex(4);

    return ppd;


}

BaseMeshRAM* BaseMeshCreator::rectangle(vec3 posLl, vec3 posUr) {
	BaseMeshRAM* rec = new BaseMeshRAM();
	rec->initialize();

	vec3 texCoordLl(0,0,0);
	vec3 texCoordUr(1,1,0);

	vec4 colorLl(1,1,1,1);
	vec4 colorUr(0,1,0,1);

	// 4 corners
	rec->addVertex(posLl, texCoordLl, colorLl);
    rec->addVertex(vec3(posLl.x, posUr.y, posLl.z), vec3(texCoordLl.x, texCoordUr.y, texCoordLl.z), vec4(colorLl.x, colorUr.y, colorLl.z, colorLl.w));
    rec->addVertex(vec3(posUr.x, posLl.y, posUr.z), vec3(texCoordLl.x, texCoordUr.y, texCoordUr.z), vec4(colorLl.x, colorUr.y, colorUr.z, colorLl.w));
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

