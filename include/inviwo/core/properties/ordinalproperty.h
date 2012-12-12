#ifndef IVW_ORDINALPROPERTY_H
#define IVW_ORDINALPROPERTY_H

#include <limits>
#include "inviwo/core/inviwo.h"
#include "inviwo/core/properties/templateproperty.h"

namespace inviwo {

template<typename T>
class OrdinalProperty : public TemplateProperty<T> {

public:
    OrdinalProperty(std::string identifier, std::string displayName, T value,
                    T minValue, T maxValue, T increment);

    T getMinValue()const ;
    T getMaxValue() const ;
    T getIncrement() const ;

    void setMinValue(const T &value) {
        minValue_ = value;
    }

    void setMaxValue(const T &value) {
        maxValue_ = value;
    }

    void setIncrement(const T &value) {
        increment_ = value;
    }

    void increase() {
        this->value_ += increment_;
        if (this->value_ > maxValue_) this->value_ = maxValue_;
        getOwner()->invalidate();
    }

    void decrease() {
        this->value_ -= increment_;
        if (this->value_ < minValue_) this->value_ = minValue_;
        getOwner()->invalidate();
    }

private:
    T minValue_;
    T maxValue_;
    T increment_;
};


template <typename T>
OrdinalProperty<T>::OrdinalProperty(std::string identifier, std::string displayName, T value,
                                    T minValue, T maxValue, T increment)
    : TemplateProperty<T>(identifier, displayName, value),
      minValue_(minValue),
      maxValue_(maxValue),
      increment_(increment)
{}

template <typename T>
T OrdinalProperty<T>::getMinValue() const { return minValue_; }

template <typename T>
T OrdinalProperty<T>::getMaxValue() const { return maxValue_; }

template <typename T>
T OrdinalProperty<T>::getIncrement() const { return increment_; }

} // namespace

#endif // IVW_ORDINALPROPERTY_H
