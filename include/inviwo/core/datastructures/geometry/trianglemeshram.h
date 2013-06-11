#ifndef IVW_TRIANGLEMESHRAM_H
#define IVW_TRIANGLEMESHRAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/geometry/meshram.h>

namespace inviwo {

class IVW_CORE_API TriangleMeshRAM : public MeshRAM {

public:
    TriangleMeshRAM();
    virtual ~TriangleMeshRAM();
    virtual void performOperation(DataOperation*) const {};
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const { return NULL; };
    virtual void render(RenderType = NOT_SPECIFIED);

    virtual unsigned int addVertex(glm::vec3& pos, glm::vec3& texCoord, glm::vec4& color);
    virtual unsigned int addIndex(unsigned int& idx);

protected:
    PositionAttributes* vertexPositions_;
    TexCoordAttributes* vertexTexCoords_;
    ColorAttributes* vertexColors_;

    IndexAttributes* faceIndices_;

};

} // namespace

#endif // IVW_TRIANGLEMESHRAM_H
