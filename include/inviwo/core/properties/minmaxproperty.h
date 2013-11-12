#ifndef IVW_MINMAXPROPERTY_H
#define IVW_MINMAXPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {

template<typename T>
class MinMaxProperty : public TemplateProperty<glm::detail::tvec2<T> > {

public:
    MinMaxProperty(std::string identifier, std::string displayName, T value, T valueMax,
        T rangeMin, T rangeMax, T increment, PropertyOwner::InvalidationLevel invalidationLevel,
        PropertySemantics::Type semantics = PropertySemantics::Default);

    T getRangeMin() const;
    T getRangeMax() const;
    T getIncrement() const;

    glm::detail::tvec2<T> getRange() const;

    void setRangeMin(const T &value);
    void setRangeMax(const T &value);
    void setIncrement(const T &value);

    void setRange(const glm::detail::tvec2<T> &value);


    virtual Variant getVariant();
    virtual void setVariant(const Variant& v);
    virtual int getVariantType();

private:
    glm::detail::tvec2<T> range_;
    T increment_;
};

template <typename T>
MinMaxProperty<T>::MinMaxProperty(std::string identifier, std::string displayName,
                                    T valueMin, T valueMax, T rangeMin, T rangeMax, T increment,
                                    PropertyOwner::InvalidationLevel invalidationLevel,
                                    PropertySemantics::Type semantics )
    : TemplateProperty<glm::detail::tvec2<T> >(identifier, displayName, glm::detail::tvec2<T>(valueMin, valueMax), invalidationLevel, semantics),
      range_(glm::detail::tvec2<T>(rangeMin, rangeMax)),
      increment_(increment)
{}

template <typename T>
T MinMaxProperty<T>::getRangeMin() const { 
    return range_.x; 
}

template <typename T>
T MinMaxProperty<T>::getRangeMax() const { 
    return range_.y; 
}

template <typename T>
T MinMaxProperty<T>::getIncrement() const { 
    return increment_; 
}

template <typename T>
glm::detail::tvec2<T> MinMaxProperty<T>::getRange() const{
    return range_;
}

template <typename T>
void MinMaxProperty<T>::setRangeMin(const T &value) {
    glm::detail::tvec2<T> val = get();
    if(val.x < value){
        val.x = value;
        set(val);
    }
    range_.x = value;
}

template <typename T>
void MinMaxProperty<T>::setRangeMax(const T &value) {
    glm::detail::tvec2<T> val = get();
    if(val.y > value){
        val.y = value;
        set(val);
    }
    range_.y = value;
}

template <typename T>
void MinMaxProperty<T>::setIncrement(const T &value) {
    increment_ = value;
}

template <typename T>
void MinMaxProperty<T>::setRange(const glm::detail::tvec2<T> &value){
    setRangeMin(value.x);
    setRangeMax(value.y);
}

template <typename T> 
Variant MinMaxProperty<T>::getVariant(){
    return Variant(TemplateProperty<glm::detail::tvec2<T> >::value_);
}

template <typename T> 
void MinMaxProperty<T>::setVariant(const Variant& v){
    if (v.canConvert(getVariantType())) {
        this->set(v.get<glm::detail::tvec2<T> >());
    }
}

template <typename T> 
int MinMaxProperty<T>::getVariantType(){
    return getVariant().getType();
}

} // namespace

#endif // IVW_MINMAXPROPERTY_H
