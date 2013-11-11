#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>

namespace inviwo {

SimpleMesh* SimpleMeshCreator::rectangularPrism(vec3 posLlf, vec3 posUrb, vec3 texCoordLlf, 
                                               vec3 texCoordUrb, vec4 colorLlf, vec4 colorUrb) {
    SimpleMesh* recPrism = new SimpleMesh();
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
    recPrism->setIndicesInfo(TRIANGLES, STRIP);
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

SimpleMesh* SimpleMeshCreator::parallelepiped(glm::vec3 pos, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, 	
											 glm::vec3 tex, glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, 	
											 glm::vec4 col, glm::vec4 c1, glm::vec4 c2, glm::vec4 c3){
   
	SimpleMesh* ppd = new SimpleMesh();
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
    ppd->setIndicesInfo(TRIANGLES, STRIP);
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

SimpleMesh* SimpleMeshCreator::rectangle(vec3 posLl, vec3 posUr) {
	SimpleMesh* rec = new SimpleMesh();
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
	rec->setIndicesInfo(TRIANGLES, STRIP);
	rec->addIndex(1);
	rec->addIndex(3);
	rec->addIndex(0);
	rec->addIndex(2);

	return rec;
}

SimpleMesh* SimpleMeshCreator::sphere(float radius, unsigned int numLoops, unsigned int segmentsPerLoop){
    SimpleMesh* spheremesh = new SimpleMesh();
    spheremesh->initialize();

    //Create Vertices
    for (unsigned int i = 0; i < segmentsPerLoop; ++i){
        float theta = 0.f;
        float phi = i * 2.f * static_cast<float>(M_PI) / segmentsPerLoop;
        float sinTheta = std::sin(theta);
        float sinPhi = std::sin(phi);
        float cosTheta = std::cos(theta);
        float cosPhi = std::cos(phi);
        vec3 vert = vec3(radius * cosPhi * sinTheta, radius * sinPhi * sinTheta, radius * cosTheta);
        spheremesh->addVertex(vert, vert, vec4(vert, 1.f));
    }
    for (unsigned int i = 0; i <= numLoops; ++i){
        for (unsigned int j = 0; j < segmentsPerLoop; ++j){
            float theta = (i * static_cast<float>(M_PI) / numLoops) + ((static_cast<float>(M_PI) * j) / (segmentsPerLoop * numLoops));
            if (i == numLoops)
                theta = static_cast<float>(M_PI);
            float phi = j * 2 * static_cast<float>(M_PI) / segmentsPerLoop;
            float sinTheta = std::sin(theta);
            float sinPhi = std::sin(phi);
            float cosTheta = std::cos(theta);
            float cosPhi = std::cos(phi);
            vec3 vert = vec3(radius * cosPhi * sinTheta, radius * sinPhi * sinTheta, radius * cosTheta);
            spheremesh->addVertex(vert, vert, vec4(vert, 1.f));

        }
    }

    //Create Indices
    spheremesh->setIndicesInfo(TRIANGLES, STRIP);
    for (unsigned int j = 0; j < segmentsPerLoop; ++j){
        spheremesh->addIndex(j);
        spheremesh->addIndex(segmentsPerLoop + j);
    }
    for (unsigned int i = 0; i < numLoops; ++i){
        for (unsigned int j = 0; j < segmentsPerLoop; ++j){
            spheremesh->addIndex(((i + 1) * segmentsPerLoop) + j);
            spheremesh->addIndex(((i + 2) * segmentsPerLoop) + j);
        }
    }

    return spheremesh;
}

} // namespace

