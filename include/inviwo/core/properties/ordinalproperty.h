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

        T getMinValue();
        T getMaxValue();

        T getIncrement();

        void increase() {
            value_ += increment_;
            if (value_ > maxValue_) value_ = maxValue_;
            getOwner()->invalidate();
        }

        void decrease() {
            value_ -= increment_;
            if (value_ < minValue_) value_ = minValue_;
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
    T OrdinalProperty<T>::getMinValue() { return minValue_; }
    template <typename T>
    T OrdinalProperty<T>::getMaxValue() { return maxValue_; }

    template <typename T>
    T OrdinalProperty<T>::getIncrement() { return increment_; }

} // namespace

#endif // IVW_ORDINALPROPERTY_H
