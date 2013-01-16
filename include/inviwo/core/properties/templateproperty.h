#ifndef IVW_TEMPLATEPROPERTY_H
#define IVW_TEMPLATEPROPERTY_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/properties/property.h"

namespace inviwo {

template<typename T>
class TemplateProperty : public Property {

public:

    TemplateProperty(std::string identifier, std::string displayName, T value);

    virtual T get() const;
    virtual void set(T value);

protected:
    T value_;
};

template <typename T>
TemplateProperty<T>::TemplateProperty(std::string identifier, std::string displayName, T value)
    : Property(identifier, displayName),
      value_(value)
{}

template <typename T>
T TemplateProperty<T>::get() const {
    return value_;
}

template <typename T>
void TemplateProperty<T>::set(T value) {
    value_ = value;
    getOwner()->invalidate();
    invalidate();
    updatePropertyWidgets();
}

} // namespace

#endif // IVW_TEMPLATEPROPERTY_H
