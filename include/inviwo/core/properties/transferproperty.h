#ifndef IVW_TRANSFERPROPERTY_H
#define IVW_TRANSFERPROPERTY_H

#include <inviwo/core/properties/templateproperty.h>
#include <inviwo/core/datastructures/transferfunc.h>

namespace inviwo {

    class IVW_CORE_API TransferProperty : public TemplateProperty<TransferFunc> {

    public:
        TransferProperty(std::string identifier, std::string displayName, TransferFunc value, PropertySemantics::Type semantics = PropertySemantics::Default);
        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& d);
    private:
        TransferFunc value;
    };
} // namespace

#endif // IVW_TRANSFERPROPERTY_H