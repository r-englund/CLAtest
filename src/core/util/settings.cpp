#include <inviwo/core/util/settings.h>

namespace inviwo {

Settings::Settings(){}

Settings::~Settings() {}

void Settings::initialize() {
}

void Settings::deinitialize() {
}

void Settings::invalidate() {
    PropertyOwner::invalidate(PropertyOwner::INVALID_OUTPUT);
}

void Settings::serialize(IvwSerializer& s) const {
    PropertyOwner::serialize(s);
}
void Settings::deserialize(IvwDeserializer& d) {
    PropertyOwner::deserialize(d);  
}

} // namespace
