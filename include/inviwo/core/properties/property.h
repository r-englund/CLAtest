#ifndef IVW_PROPERTY_H
#define IVW_PROPERTY_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/properties/propertywidget.h>
#include <inviwo/core/util/variant.h>

namespace inviwo {

class IVW_CORE_API Property : public IvwSerializable {

public:
    Property(std::string identifier, std::string displayName);
    Property();

    virtual std::string getIdentifier() const;
    virtual void setIdentifier(const std::string& identifier);

    virtual std::string getDisplayName() const;
    virtual void setDisplayName(const std::string& displayName);

    PropertyOwner* getOwner();
    virtual void setOwner(PropertyOwner* owner);

    void registerPropertyWidget(PropertyWidget* propertyWidget);
    void updatePropertyWidgets();

    void invalidate();
    bool isValid();
    void setValid();

    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);

private:
    std::string identifier_;
    std::string displayName_;
    PropertyOwner* owner_;
    std::vector<PropertyWidget*> propertyWidgets_;
    bool invalid_;
};

} // namespace

#endif // IVW_PROPERTY_H