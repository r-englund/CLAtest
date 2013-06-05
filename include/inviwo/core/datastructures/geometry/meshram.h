#ifndef IVW_MESHRAM_H
#define IVW_MESHRAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/attributes.h>
#include <inviwo/core/datastructures/geometry/geometryram.h>

namespace inviwo {

class IVW_CORE_API MeshRAM : public GeometryRAM {

public:
    MeshRAM();
    virtual ~MeshRAM();
    virtual void performOperation(DataOperation*) const = 0;
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const = 0;

    virtual unsigned int addVertex(const glm::vec3& pos, const glm::vec3& texCoord, const glm::vec3& color);

protected:
    std::vector<AttributesBase*> vertexAttributes_;
    std::vector<AttributesBase*> faceAttributes_;

};

} // namespace

#endif // IVW_MESHRAM_H
