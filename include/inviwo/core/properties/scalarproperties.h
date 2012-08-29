#ifndef IVW_SCALARPROPERTIES_H
#define IVW_SCALARPROPERTIES_H

#include "inviwo/core/properties/ordinalproperty.h"

namespace inviwo {

class FloatProperty : public OrdinalProperty<float> {
public:
    FloatProperty(std::string identifier, std::string displayName, float value, float minValue=0.0f, float maxValue=1.0f, float increment=0.1f);
};

class IntProperty : public OrdinalProperty<int> {
public:
    IntProperty(std::string identifier, std::string displayName, int value, int minValue=0, int maxValue=4096, int increment=1);
};

} // namespace

#endif // IVW_SCALARPROPERTIES_H