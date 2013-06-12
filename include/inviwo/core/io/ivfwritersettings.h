#ifndef IVW_IVFWRITERSETTINGS_H
#define IVW_IVFWRITERSETTINGS_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/common/inviwo.h"
#include "inviwo/core/io/ivfbasesettings.h"
#include "inviwo/core/io/volumewriter.h"

namespace inviwo {

class IVW_CORE_API IvfWriterSettings : public WriterSettings, public IvfBaseSettings {
public:
    IvfWriterSettings();
    ~IvfWriterSettings() {}

    //serialization
    virtual void serialize(IvwSerializer& s) const;

    //de-serialization
    virtual void deserialize(IvwDeserializer& d);
};

} // namespace

#endif // IVW_IVFWRITERSETTINGS_H
