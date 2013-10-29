#ifndef IVW_MESHRAM_H
#define IVW_MESHRAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/attributes.h>
#include <inviwo/core/datastructures/geometry/geometryram.h>
#include <utility>

namespace inviwo {

class IVW_CORE_API Mesh : public Geometry {

public:
    struct AttributesInfo {
        RenderType rt;
        ConnectivityType ct;
        AttributesInfo() : rt(POINTS), ct(NONE){}
        AttributesInfo(RenderType r, ConnectivityType c) : rt(r), ct(c){}
    };

    Mesh();
    Mesh(RenderType rt, ConnectivityType ct);
    virtual ~Mesh();
    virtual void performOperation(DataOperation*) const {};
    virtual void initialize();
    virtual void deinitialize();
    virtual Data* clone() const;

    void addAttribute(Buffer*);
    void addIndicies(AttributesInfo info, IndexBuffer* ind);

    const std::vector<Buffer*>& getBuffers() const { return attributes_; }
    const std::vector<std::pair<AttributesInfo, IndexBuffer*> >& getIndexBuffers() const { return indexAttributes_; }

    Buffer* getAttributes(size_t idx) const;
    Buffer* getIndicies(size_t idx) const;

    AttributesInfo getAttributesInfo() const;
    AttributesInfo getIndexAttributesInfo(size_t idx) const;

    size_t getNumberOfAttributes() const;
    size_t getNumberOfIndicies() const;

protected:
    std::vector<Buffer*> attributes_;
    AttributesInfo attributesInfo_;
    std::vector<std::pair<AttributesInfo, IndexBuffer*> > indexAttributes_;

};


} // namespace

#endif // IVW_MESHRAM_H
