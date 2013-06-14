#ifndef IVW_TRIANGLEMESHRAM_H
#define IVW_TRIANGLEMESHRAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/geometry/meshram.h>

namespace inviwo {

class IVW_CORE_API BaseMeshRAM : public MeshRAM {

public:
    BaseMeshRAM(RenderType rt = GeometryRepresentation::POINTS, ConnectivityType ct = GeometryRepresentation::NONE);
    virtual ~BaseMeshRAM();
    virtual void performOperation(DataOperation*) const {};
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const { return NULL; };
    virtual void render() const;

    void addVertex(glm::vec3 pos, glm::vec3 texCoord, glm::vec4 color);
    void addIndex(unsigned int idx);
    void setIndicesInfo(RenderType, ConnectivityType);

protected:
    PositionAttributes* vertexPositions_;
    TexCoordAttributes* vertexTexCoords_;
    ColorAttributes* vertexColors_;
    IndexAttributes* indices_;
};

} // namespace

#endif // IVW_TRIANGLEMESHRAM_H
