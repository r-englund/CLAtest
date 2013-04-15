#ifndef IVW_TEMPLATEPROPERTY_H
#define IVW_TEMPLATEPROPERTY_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
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
    virtual void set(T value);

protected:
    T value_;
};

template <typename T>
TemplateProperty<T>::TemplateProperty(std::string identifier, std::string displayName, T value,
                                      PropertyOwner::InvalidationLevel invalidationLevel,
                                      PropertySemantics::Type semantics = PropertySemantics::Default)
: Property(identifier, displayName, invalidationLevel, semantics),
value_(value)
{}

template <typename T>
T& TemplateProperty<T>::get() {
    return value_;
}

template <typename T>
void TemplateProperty<T>::set(T value) {
    value_ = value;
    onChangeCallback_.invoke();
    getOwner()->invalidate();
    invalidate();
    updatePropertyWidgets();
    notifyObservers();
}

} // namespace

#endif // IVW_TEMPLATEPROPERTY_H
