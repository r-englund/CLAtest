#ifndef IVW_TEMPLATEPROPERTY_H
#define IVW_TEMPLATEPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/property.h>

namespace inviwo {

template<typename T>
class IVW_CORE_API TemplateProperty : public Property {

public:

    TemplateProperty(std::string identifier, std::string displayName, T value,
                     PropertyOwner::InvalidationLevel invalidationLevel = PropertyOwner::INVALID_OUTPUT,
                     PropertySemantics::Type semantics = PropertySemantics::Default);

    virtual T& get();
	virtual const T& get() const { return value_; };
    virtual void set(const T& value);

protected:
    T value_;
};

template <typename T>
TemplateProperty<T>::TemplateProperty(std::string identifier, std::string displayName, T value,
                                      PropertyOwner::InvalidationLevel invalidationLevel,
                                      PropertySemantics::Type semantics)
: Property(identifier, displayName, invalidationLevel, semantics),
value_(value)
{}

template <typename T>
T& TemplateProperty<T>::get() {
    return value_;
}

template <typename T>
void TemplateProperty<T>::set(const T& value) {
    value_ = value;
    onChangeCallback_.invoke();
    getOwner()->invalidate();
    updatePropertyWidgets();
}

} // namespace

#endif // IVW_TEMPLATEPROPERTY_H
