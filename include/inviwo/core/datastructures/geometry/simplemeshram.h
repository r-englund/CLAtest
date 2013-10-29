#ifndef IVW_SIMPLEMESHRAM_H
#define IVW_SIMPLEMESHRAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/geometry/meshram.h>

namespace inviwo {

class IVW_CORE_API SimpleMeshRAM : public Mesh {

public:
    SimpleMeshRAM(RenderType rt = POINTS, ConnectivityType ct = NONE);
    virtual ~SimpleMeshRAM();
    virtual void performOperation(DataOperation*) const {};
    virtual void initialize();
    virtual void deinitialize();

    void addVertex(vec3 pos, vec3 texCoord, vec4 color);
    void addIndex(unsigned int idx);
    void setIndicesInfo(RenderType, ConnectivityType);
	const Position3dBuffer* getVertexList() const;
	const IndexBuffer* getIndexList() const;

protected:
    Position3dBuffer* vertexPositions_;
    TexCoord3dBuffer* vertexTexCoords_;
    ColorBuffer* vertexColors_;
    IndexBuffer* indices_;
};

} // namespace

#endif // IVW_SIMPLEMESHRAM_H
