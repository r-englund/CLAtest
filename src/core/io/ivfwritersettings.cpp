#include "inviwo/core/io/ivfwritersettings.h"

namespace inviwo {

IvfWriterSettings::IvfWriterSettings() 
    : WriterSettings(),
      IvfBaseSettings()    
{}


void IvfWriterSettings::serialize(IvwSerializer& s) const {
    WriterSettings::serialize(s);
    IvfBaseSettings::serialize(s);
}

void IvfWriterSettings::deserialize(IvwDeserializer& d) {
    WriterSettings::deserialize(d);
    IvfBaseSettings::deserialize(d);
}

} // namespace
