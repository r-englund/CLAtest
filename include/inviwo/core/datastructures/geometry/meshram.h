#ifndef IVW_MESHRAM_H
#define IVW_MESHRAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/attributes.h>
#include <inviwo/core/datastructures/geometry/geometryram.h>
#include <utility>

namespace inviwo {

class IVW_CORE_API MeshRAM : public GeometryRAM {

public:
    struct AttributesInfo{
        GeometryRepresentation::RenderType rt;
        GeometryRepresentation::ConnectivityType ct;
        AttributesInfo() : rt(GeometryRepresentation::POINTS), ct(GeometryRepresentation::NONE){}
        AttributesInfo(GeometryRepresentation::RenderType r, GeometryRepresentation::ConnectivityType c) : rt(r), ct(c){}
    };

    MeshRAM();
    MeshRAM(RenderType rt, ConnectivityType ct);
    virtual ~MeshRAM();
    virtual void performOperation(DataOperation*) const {};
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const { return NULL; };
    virtual void render(RenderType = GeometryRepresentation::NOT_SPECIFIED) const{};

    AttributesBase* getAttributes(size_t idx) const;
    AttributesBase* getIndicies(size_t idx) const;

    AttributesInfo getAttributesInfo() const;
    AttributesInfo getIndexAttributesInfo(size_t idx) const;

    size_t getNumberOfAttributes() const;
    size_t getNumberOfIndicies() const;

protected:
    std::vector<AttributesBase*> attributes_;
    AttributesInfo attributesInfo_;
    std::vector<std::pair<AttributesInfo, IndexAttributes*> > indexAttributes_;

};

} // namespace

#endif // IVW_MESHRAM_H
