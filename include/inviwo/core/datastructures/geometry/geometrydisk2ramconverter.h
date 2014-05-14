#ifndef IVW_GEOMETRYDISK2RAMCONVERTER_H
#define IVW_GEOMETRYDISK2RAMCONVERTER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/datastructures/geometry/geometryram.h>
#include <inviwo/core/datastructures/geometry/meshdisk.h>


namespace inviwo {

class IVW_CORE_API GeometryDisk2RAMConverter : public RepresentationConverterType<GeometryRAM> {
public:
    GeometryDisk2RAMConverter();
    virtual ~GeometryDisk2RAMConverter();

    inline bool canConvertFrom(const DataRepresentation* source) const {
        return dynamic_cast<const MeshDisk*>(source) != NULL;
    }

    DataRepresentation* createFrom(const DataRepresentation* source);
    void update(const DataRepresentation* source, DataRepresentation* destination);
};

} // namespace

#endif // IVW_GEOMETRYDISK2RAMCONVERTER_H

