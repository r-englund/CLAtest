#include "inviwo/core/io/ivfreadersettings.h"

namespace inviwo {

IvfReaderSettings::IvfReaderSettings() 
    : ReaderSettings(),
      IvfBaseSettings()
{}

void IvfReaderSettings::serialize(IvwSerializer& s) const {
    ReaderSettings::serialize(s);
    IvfBaseSettings::serialize(s);
}

void IvfReaderSettings::deserialize(IvwDeserializer& d) {
    ReaderSettings::deserialize(d);
    IvfBaseSettings::deserialize(d);
}

} // namespace
