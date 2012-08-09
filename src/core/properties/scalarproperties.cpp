#include "inviwo/core/properties/scalarproperties.h"

namespace inviwo {

    FloatProperty::FloatProperty(std::string identifier, std::string displayName, float value,
        float minValue, float maxValue, float increment)
        : OrdinalProperty(identifier, displayName, value, minValue, maxValue, increment)
    {}

    IntProperty::IntProperty(std::string identifier, std::string displayName, int value,
        int minValue, int maxValue, int increment)
        : OrdinalProperty(identifier, displayName, value, minValue, maxValue, increment)
    {}

} // namespace
