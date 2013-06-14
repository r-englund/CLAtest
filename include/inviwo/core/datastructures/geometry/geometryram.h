#ifndef IVW_GEOMETRYRAM_H
#define IVW_GEOMETRYRAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/geometryrepresentation.h>

namespace inviwo {

class IVW_CORE_API GeometryRAM : public GeometryRepresentation {

public:
    GeometryRAM();
    virtual ~GeometryRAM();
    virtual void performOperation(DataOperation*) const = 0;
    virtual void initialize();
    virtual void deinitialize();
    virtual DataRepresentation* clone() const = 0;
    virtual void render(RenderType = GeometryRepresentation::NOT_SPECIFIED) const = 0;
};

} // namespace

#endif // IVW_GEOMETRYRAM_H
