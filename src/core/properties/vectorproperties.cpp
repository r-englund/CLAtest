#include "inviwo/core/properties/vectorproperties.h"

namespace inviwo {

    FloatVec3Property::FloatVec3Property(std::string identifier, std::string displayName, vec3 value,
                                         vec3 minValue, vec3 maxValue, vec3 increment)
        : OrdinalProperty(identifier, displayName, value, minValue, maxValue, increment)
    {}

    void FloatVec3Property::serialize(IvwSerializer& s) const {
        Property::serialize(s) ;
        s.serialize("value", get());
        s.serialize("minvalue", OrdinalProperty<vec3>::getMinValue());
        s.serialize("maxvalue", OrdinalProperty<vec3>::getMaxValue());
        s.serialize("increment", OrdinalProperty<vec3>::getIncrement());
    }

    void FloatVec3Property::deserialize(IvwDeserializer& d) {
        Property::deserialize(d) ;
        vec3 value;
        d.deserialize("value", value);
        set(value);

        d.deserialize("minvalue",  value);
        OrdinalProperty<vec3>::setMinValue(value);

        d.deserialize("maxvalue", value);
        OrdinalProperty<vec3>::setMaxValue(value);

        d.deserialize("increment", value);
        OrdinalProperty<vec3>::setIncrement(value);
    }

} // namespace
