#ifndef IVW_BASEMESHCREATOR_H
#define IVW_BASEMESHCREATOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/trianglemeshram.h>

namespace inviwo {

class IVW_CORE_API BaseMeshCreator {

public:
    static TriangleMeshRAM* rectangularPrism(glm::vec3 posLlf, glm::vec3 posUrb, glm::vec3 texCoordLlf, glm::vec3 texCoordUrb, 
                                 glm::vec3 colorLlf, glm::vec3 colorUrb, float alpha);

};

} // namespace

#endif // IVW_BASEMESHCREATOR_H
