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
    virtual void render(){};

    AttributesBase* getAttribute(size_t idx) const;
    AttributesBase* getIndicies() const;

    size_t getNumberOfAttributes() const;

protected:
    std::vector<AttributesBase*> attributes_;
    IndexAttributes* indices_;

};

} // namespace

#endif // IVW_MESHRAM_H
