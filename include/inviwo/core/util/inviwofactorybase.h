#ifndef IVW_INVIWOFACTORYBASE_H
#define IVW_INVIWOFACTORYBASE_H

#include <inviwo/core/inviwocoredefine.h>
#include <string>

namespace inviwo {

class IvwSerializable;

class IVW_CORE_API Factory {

public:
    ~Factory() {}

    virtual void initialize()=0;
    virtual void deinitialize()=0;

    virtual IvwSerializable* create(std::string className) const { return 0; }
    virtual bool isValidType(std::string className) const {return false; }
};

} // namespace

#endif // IVW_INVIWOFACETORYBASE_H
