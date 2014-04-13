#ifndef IVW_BASICMESH_H
#define IVW_BASICMESH_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>

namespace inviwo {

class IVW_CORE_API BasicMesh : public Mesh {
public:
    BasicMesh();
    virtual ~BasicMesh();
    
    void addVertex(vec3 pos, vec3 normal, vec3 texCoord, vec4 color);
    IndexBufferRAM* addIndexBuffer(RenderType rt, ConnectivityType ct);

    const Position3dBuffer* getVertices() const;
    const TexCoord3dBuffer* getTexCoords() const;
    const ColorBuffer* getColors() const;
    const NormalBuffer* getNormals() const;
    
    void append(const BasicMesh* mesh);
    
    static vec3 orthvec(const vec3& vec);
    static BasicMesh* disk(const vec3& center,
                           const vec3& normal,
                           const vec4& color,
                           const float& radius,
                           const size_t& segments=16);
    static BasicMesh* cone(const vec3& start,
                           const vec3& stop,
                           const vec4& color,
                           const float& radius,
                           const size_t& segments=16);
    static BasicMesh* cylinder(const vec3& start,
                               const vec3& stop,
                               const vec4& color,
                               const float& radius,
                               const size_t& segments=16);
    static BasicMesh* arrow(const vec3& start,
                            const vec3& stop,
                            const vec4& color,
                            const float& radius,
                            const float& arrowfraction,
                            const float& arrowRadius,
                            const size_t& segemnts=16);
    
    
protected:
    Position3dBuffer* vertices_;
    TexCoord3dBuffer* texCoords_;
    ColorBuffer* colors_;
    NormalBuffer* normals_;
};

} // namespace

#endif // IVW_BASICMESH_H

