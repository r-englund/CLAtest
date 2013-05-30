#ifndef IVW_PROPERTY_H
#define IVW_PROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/properties/propertywidget.h>
#include <inviwo/core/properties/propertysemantics.h>
#include <inviwo/core/util/callback.h>
#include <inviwo/core/util/variant.h>
#include <inviwo/core/util/observer.h>

namespace inviwo {

class IVW_CORE_API Property : public IvwSerializable, public VoidObservable {

public:
    Property(std::string identifier, std::string displayName,
             PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
             PropertySemantics::Type semantics=PropertySemantics::Default);
    Property();
    
    virtual std::string getIdentifier() const;
    virtual void setIdentifier(const std::string& identifier);

    virtual std::string getDisplayName() const;
    virtual void setDisplayName(const std::string& displayName);

    virtual PropertySemantics::Type getSemantics() const;
    virtual void setSemantics(const PropertySemantics::Type& semantics);

    PropertyOwner::InvalidationLevel getInvalidationLevel() const { return invalidationLevel_; }
    void setInvalidationLevel(PropertyOwner::InvalidationLevel invalidationLevel) { invalidationLevel_ = invalidationLevel; }

    PropertyOwner* getOwner();
    virtual void setOwner(PropertyOwner* owner);

    void registerPropertyWidget(PropertyWidget* propertyWidget);
    void updatePropertyWidgets();

    void invalidate();
    bool isValid();
    void setValid();

    void setVisible(bool visible) { visible_ = visible; };

    virtual Variant getVariant();
    virtual void setVariant(const Variant&);
    virtual int getVariantType();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);

    template <typename T>
    void onChange(T* o, void (T::*m)()) {
        onChangeCallback_.addMemberFunction(o,m);
    }

protected:
    SingleCallBack onChangeCallback_;

private:
    std::string identifier_;
    std::string displayName_;
    PropertyOwner::InvalidationLevel invalidationLevel_;
    PropertySemantics::Type semantics_;

    bool invalid_;
    PropertyOwner* owner_;
    std::vector<PropertyWidget*> propertyWidgets_;

    bool visible_;
};

} // namespace

#endif // IVW_PROPERTY_H