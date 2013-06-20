#ifndef IVW_MINMAXPROPERTY_H
#define IVW_MINMAXPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {

template<typename T>
class IVW_CORE_API MinMaxProperty : public TemplateProperty<glm::detail::tvec2<T> > {

public:
    MinMaxProperty(std::string identifier, std::string displayName, T value, T valueMax,
        T rangeMin, T rangeMax, T increment, PropertyOwner::InvalidationLevel invalidationLevel,
        PropertySemantics::Type semantics = PropertySemantics::Default);

    T getValueMin() const;
    T getValueMax() const;
    T getRangeMin() const;
    T getRangeMax() const;
    T getIncrement() const;

    glm::detail::tvec2<T> getRange() const;

    void setValueMin(const T &value);
    void setValueMax(const T &value);
    void setRangeMin(const T &value);
    void setRangeMax(const T &value);
    void setIncrement(const T &value);

    void setRange(const glm::detail::tvec2<T> &value);

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
T MinMaxProperty<T>::getValueMin() const { 
    return get().x; 
}

template <typename T>
T MinMaxProperty<T>::getValueMax() const { 
    return get().y; 
}

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
void MinMaxProperty<T>::setValueMin(const T &value) {
    set(glm::detail::tvec2<T>(value, get().y));
}

template <typename T>
void MinMaxProperty<T>::setValueMax(const T &value) {
    set(glm::detail::tvec2<T>(get().x, value));
}

template <typename T>
void MinMaxProperty<T>::setRangeMin(const T &value) {
    range_.x = value;
}

template <typename T>
void MinMaxProperty<T>::setRangeMax(const T &value) {
    range_.y = value;
}

template <typename T>
void MinMaxProperty<T>::setIncrement(const T &value) {
    increment_ = value;
}

template <typename T>
void MinMaxProperty<T>::setRange(const glm::detail::tvec2<T> &value){
    range_ = value;
}

} // namespace

#endif // IVW_MINMAXPROPERTY_H
