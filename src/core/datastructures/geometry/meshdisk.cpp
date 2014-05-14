#include <inviwo/core/datastructures/geometry/meshdisk.h>

namespace inviwo {

MeshDisk::MeshDisk() : GeometryDisk() {}

MeshDisk::MeshDisk(std::string srcFile) : GeometryDisk(srcFile) {}

MeshDisk::MeshDisk(const MeshDisk& rhs) : GeometryDisk(rhs) {}

MeshDisk& MeshDisk::operator=(const MeshDisk& that) {
    if (this != &that) {
        GeometryDisk::operator=(that);
    }
    return *this;
}

MeshDisk* MeshDisk::clone() const {
    return new MeshDisk(*this);
}

void MeshDisk::initialize() {}

void MeshDisk::deinitialize() {}

void MeshDisk::update(bool editable) {}

}  // namespace
