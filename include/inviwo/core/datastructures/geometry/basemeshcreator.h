#ifndef IVW_BASEMESHCREATOR_H
#define IVW_BASEMESHCREATOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/basemeshram.h>

namespace inviwo {

class IVW_CORE_API BaseMeshCreator {

public:
    static BaseMeshRAM* rectangularPrism(vec3 posLlf, vec3 posUrb, vec3 texCoordLlf, 
                                        vec3 texCoordUrb, vec4 colorLlf, vec4 colorUrb);
	static BaseMeshRAM* parallelepiped(glm::vec3 pos, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, 	
									   glm::vec3 tex, glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, 	
									   glm::vec4 col, glm::vec4 c1, glm::vec4 c2, glm::vec4 c3); 	

	static BaseMeshRAM* rectangle(glm::vec3 posLl, glm::vec3 posUr);
};

} // namespace

#endif // IVW_BASEMESHCREATOR_H
