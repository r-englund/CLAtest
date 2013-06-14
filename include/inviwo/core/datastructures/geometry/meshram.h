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
    virtual void performOperation(DataOperation*) const {};
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const { return NULL; };
    virtual void render() const{};

    AttributesBase* getAttributes(size_t idx) const;
    AttributesBase* getIndicies(size_t idx) const;
    Connectivity getIndexConnectivity(size_t idx) const;

    size_t getNumberOfAttributes() const;
    size_t getNumberOfIndicies() const;

protected:
    std::vector<AttributesBase*> attributes_;
    std::vector<std::pair<Connectivity, IndexAttributes*>> indices_;

};

} // namespace

#endif // IVW_MESHRAM_H
