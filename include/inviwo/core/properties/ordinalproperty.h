#ifndef IVW_ORDINALPROPERTY_H
#define IVW_ORDINALPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {

template<typename T>
class IVW_CORE_API OrdinalProperty : public TemplateProperty<T> {

public:
    OrdinalProperty(std::string identifier, std::string displayName, T value,
        T minValue, T maxValue, T increment, PropertyOwner::InvalidationLevel invalidationLevel,
        PropertySemantics::Type semantics = PropertySemantics::Default);

    T getMinValue() const;
    T getMaxValue() const;
    T getIncrement() const;

    void setMinValue(const T &value);
    void setMaxValue(const T &value);
    void setIncrement(const T &value);

    virtual Variant getVariant();
    virtual void setVariant(const Variant& v);
    virtual int getVariantType();


private:
    T minValue_;
    T maxValue_;
    T increment_;
};


template <typename T>
OrdinalProperty<T>::OrdinalProperty(std::string identifier, std::string displayName, T value,
                                    T minValue, T maxValue, T increment,
                                    PropertyOwner::InvalidationLevel invalidationLevel,
                                    PropertySemantics::Type semantics )
    : TemplateProperty<T>(identifier, displayName, value, invalidationLevel, semantics),
      minValue_(minValue),
      maxValue_(maxValue),
      increment_(increment)
{}

template <typename T>
T OrdinalProperty<T>::getMinValue() const { 
    return minValue_; 
}

template <typename T>
T OrdinalProperty<T>::getMaxValue() const { 
    return maxValue_; 
}

template <typename T>
T OrdinalProperty<T>::getIncrement() const { 
    return increment_; 
}

template <typename T>
void OrdinalProperty<T>::setMinValue(const T &value) {
    minValue_ = value;
}

template <typename T>
void OrdinalProperty<T>::setMaxValue(const T &value) {
    maxValue_ = value;
}

template <typename T>
void OrdinalProperty<T>::setIncrement(const T &value) {
    increment_ = value;
}

template <typename T>
Variant OrdinalProperty<T>::getVariant(){
    return Variant(TemplateProperty<T>::value_);
}

template <typename T>
void OrdinalProperty<T>::setVariant(const Variant& v){
    if (v.canConvert(getVariantType())) {
        this->set(v.get<T>());
    }
}

template <typename T>
int OrdinalProperty<T>::getVariantType(){
    return getVariant().getType();
}


} // namespace

#endif // IVW_ORDINALPROPERTY_H
