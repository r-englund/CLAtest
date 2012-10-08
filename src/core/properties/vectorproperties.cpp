#include "inviwo/core/properties/vectorproperties.h"

namespace inviwo {

    FloatVec3Property::FloatVec3Property(std::string identifier, std::string displayName, vec3 value,
                                         vec3 minValue, vec3 maxValue, vec3 increment)
        : OrdinalProperty(identifier, displayName, value, minValue, maxValue, increment)
    {}

    void FloatVec3Property::serialize(IvwSerializer& s) const {
        Property::serialize(s) ;
        s.serialize("f_value", get());
        s.serialize("f_minvalue", OrdinalProperty<vec3>::getMinValue());
        s.serialize("f_maxvalue", OrdinalProperty<vec3>::getMaxValue());
        s.serialize("f_incrementvalue", OrdinalProperty<vec3>::getIncrement());
    }

    void FloatVec3Property::deserialize(IvwDeserializer& d) {
        Property::deserialize(d) ;
        vec3 value;
        d.deserialize("f_value", value);
        set(value);

        d.deserialize("f_minvalue",  value);
        OrdinalProperty<vec3>::setMinValue(value);

        d.deserialize("f_maxvalue", value);
        OrdinalProperty<vec3>::setMinValue(value);

        d.deserialize("f_incrementvalue", value);
        OrdinalProperty<vec3>::setMinValue(value);
    }

} // namespace
