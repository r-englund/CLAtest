#ifndef IVW_TRANSFERPROPERTY_H
#define IVW_TRANSFERPROPERTY_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {

class IVW_CORE_API TransferProperty : public TemplateProperty<bool> {

public:
    TransferProperty(std::string identifier, std::string displayName, bool value);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

} // namespace

#endif // IVW_TRANSFERPROPERTY_H