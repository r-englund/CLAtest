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
	static BaseMeshRAM* rectangle(vec3 posLl, vec3 posUr);
};

} // namespace

#endif // IVW_BASEMESHCREATOR_H
