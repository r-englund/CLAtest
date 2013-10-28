#ifndef IVW_MESHREPRESENTATION_H
#define IVW_MESHREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/datarepresentation.h>
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API MeshRepresentation : public DataRepresentation {

public:


    MeshRepresentation();
    virtual ~MeshRepresentation();
    virtual void performOperation(DataOperation*) const;
    virtual DataRepresentation* clone() const = 0;
    virtual std::string getClassName() const;
};

} // namespace

#endif // IVW_MESHREPRESENTATION_H
