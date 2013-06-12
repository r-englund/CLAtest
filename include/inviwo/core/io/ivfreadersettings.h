#ifndef IVW_IVFREADERSETTINGS_H
#define IVW_IVFREADERSETTINGS_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/common/inviwo.h"
#include "inviwo/core/io/ivfbasesettings.h"
#include "inviwo/core/io/volumereader.h"

namespace inviwo {

class IVW_CORE_API IvfReaderSettings : public ReaderSettings, public IvfBaseSettings {
public:
    IvfReaderSettings();
     ~IvfReaderSettings() {};

    //serialization
    void serialize(IvwSerializer& s) const;

    //de-serialization
    void deserialize(IvwDeserializer& d);
}; 

} // namespace

#endif // IVW_IVFREADERSETTINGS_H
