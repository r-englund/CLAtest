#ifndef IVW_GEOMETRYDISK_H
#define IVW_GEOMETRYDISK_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/geometryrepresentation.h>
#include <inviwo/core/datastructures/diskrepresentation.h>

namespace inviwo {

class IVW_CORE_API GeometryDisk : public GeometryRepresentation, public DiskRepresentation {
public:
    GeometryDisk();
    GeometryDisk(std::string srcFile);
    GeometryDisk(const GeometryDisk& rhs);
    GeometryDisk& operator=(const GeometryDisk& that);
    virtual ~GeometryDisk() {}
};

}  // namespace

#endif  // IVW_GEOMETRYDISK_H
