#include <inviwo/core/util/settings.h>

namespace inviwo {

const std::string Settings::logSource_ = "Settings";

Settings::Settings(){}

Settings::~Settings() {}

void Settings::initialize() {
}

void Settings::deinitialize() {
}

void Settings::invalidate() {
    PropertyOwner::invalidate();
}

void Settings::serialize(IvwSerializer& s) const {
    PropertyOwner::serialize(s);
}
void Settings::deserialize(IvwDeserializer& d) {
    PropertyOwner::deserialize(d);  
}

} // namespace
