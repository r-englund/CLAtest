#include <inviwo/core/datastructures/geometry/geometrydisk.h>

namespace inviwo {

GeometryDisk::GeometryDisk() : GeometryRepresentation(), DiskRepresentation() {}

GeometryDisk::GeometryDisk(std::string srcFile)
    : GeometryRepresentation(), DiskRepresentation(srcFile) {}

GeometryDisk::GeometryDisk(const GeometryDisk& rhs)
    : GeometryRepresentation(rhs), DiskRepresentation(rhs) {}

GeometryDisk& GeometryDisk::operator=(const GeometryDisk& that) {
    if (this != &that) {
        GeometryRepresentation::operator=(that);
        DiskRepresentation::operator=(that);
    }
    return *this;
}

}  // namespace
