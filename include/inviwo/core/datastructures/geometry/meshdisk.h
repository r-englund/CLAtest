#ifndef IVW_MESHDISK_H
#define IVW_MESHDISK_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/geometry/geometrydisk.h>

namespace inviwo {

class IVW_CORE_API MeshDisk : public GeometryDisk {
public:
    MeshDisk();
    MeshDisk(std::string srcFile);
    MeshDisk(const MeshDisk& rhs);
    virtual ~MeshDisk(){}
    MeshDisk& operator=(const MeshDisk& that);
    virtual MeshDisk* clone() const;

    virtual void initialize();
    virtual void deinitialize();

protected:
    virtual void update(bool editable);
};

} // namespace

#endif // IVW_MESHDISK_H

