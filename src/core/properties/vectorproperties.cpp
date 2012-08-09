#include "inviwo/core/properties/vectorproperties.h"

namespace inviwo {

    FloatVec3Property::FloatVec3Property(std::string identifier, std::string displayName, vec3 value,
                                         vec3 minValue, vec3 maxValue, vec3 increment)
        : OrdinalProperty(identifier, displayName, value, minValue, maxValue, increment)
    {}

} // namespace
