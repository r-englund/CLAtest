#ifndef IVW_FILEPROPERTY_H
#define IVW_FILEPROPERTY_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/properties/templateproperty.h"

namespace inviwo {

class IVW_CORE_API FileProperty : public TemplateProperty<std::string> {

public:
    FileProperty(std::string identifier, std::string displayName, std::string value);
    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

} // namespace

#endif // IVW_FILEPROPERTY_H