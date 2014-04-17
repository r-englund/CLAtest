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

    virtual std::string getDataInfo() const;

    const Position3dBuffer* getVertices() const;
    const TexCoord3dBuffer* getTexCoords() const;
    const ColorBuffer* getColors() const;
    const NormalBuffer* getNormals() const;
    
    void append(const BasicMesh* mesh);
    

    static BasicMesh* disk(const vec3& center,
                           const vec3& normal,
                           const vec4& color = vec4(1.0f,0.0f,0.0f,1.0f),
                           const float& radius = 1.0f,
                           const size_t& segments=16);
    static BasicMesh* cone(const vec3& start,
                           const vec3& stop,
                           const vec4& color = vec4(1.0f, 0.0f, 0.0f, 1.0f),
                           const float& radius = 1.0f,
                           const size_t& segments=16);
    static BasicMesh* cylinder(const vec3& start,
                               const vec3& stop,
                               const vec4& color = vec4(1.0f, 0.0f, 0.0f, 1.0f),
                               const float& radius = 1.0f,
                               const size_t& segments=16);
    static BasicMesh* arrow(const vec3& start,
                            const vec3& stop,
                            const vec4& color = vec4(1.0f, 0.0f, 0.0f, 1.0f),
                            const float& radius = 1.0f,
                            const float& arrowfraction = 0.15f,
                            const float& arrowRadius = 2.0f,
                            const size_t& segments=16);

    static BasicMesh* colorsphere(const vec3& center,
                                  const float& radius);


    static BasicMesh* coordindicator(const vec3& center,
                                     const float& size);

    static BasicMesh* boundingbox(const mat4& basisandoffset, const vec4& color);

    
    
protected:
    static vec3 orthvec(const vec3& vec);
    static vec3 calcnormal(const vec3& r, const vec3& p);
    static vec3 tospherical(const vec2& v);

    Position3dBuffer* vertices_;
    TexCoord3dBuffer* texCoords_;
    ColorBuffer* colors_;
    NormalBuffer* normals_;
};

} // namespace

#endif // IVW_BASICMESH_H

