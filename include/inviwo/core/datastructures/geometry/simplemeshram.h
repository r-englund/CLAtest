#ifndef IVW_SIMPLEMESHRAM_H
#define IVW_SIMPLEMESHRAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/geometry/meshram.h>

namespace inviwo {

class IVW_CORE_API SimpleMeshRAM : public MeshRAM {

public:
    SimpleMeshRAM(RenderType rt = GeometryRepresentation::POINTS, ConnectivityType ct = GeometryRepresentation::NONE);
    virtual ~SimpleMeshRAM();
    virtual void performOperation(DataOperation*) const {};
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const { return NULL; };
    virtual void render() const;

    void addVertex(vec3 pos, vec3 texCoord, vec4 color);
    void addIndex(unsigned int idx);
    void setIndicesInfo(RenderType, ConnectivityType);
	Position3dAttributes* getVertexList();
	IndexAttributes* getIndexList();

protected:
    Position3dAttributes* vertexPositions_;
    TexCoord3dAttributes* vertexTexCoords_;
    ColorAttributes* vertexColors_;
    IndexAttributes* indices_;
};

} // namespace

#endif // IVW_SIMPLEMESHRAM_H
