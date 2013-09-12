#ifndef IVW_BASEMESHCREATOR_H
#define IVW_BASEMESHCREATOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/basemeshram.h>

namespace inviwo {

class IVW_CORE_API BaseMeshCreator {

public:
    static BaseMeshRAM* rectangularPrism(glm::vec3 posLlf, glm::vec3 posUrb, glm::vec3 texCoordLlf, 
                                        glm::vec3 texCoordUrb, glm::vec4 colorLlf, glm::vec4 colorUrb);
	static BaseMeshRAM* rectangle(glm::vec3 posLl, glm::vec3 posUr);
};

} // namespace

#endif // IVW_BASEMESHCREATOR_H
