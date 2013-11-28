#ifndef IVW_IVFREADERSETTINGS_H
#define IVW_IVFREADERSETTINGS_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/common/inviwo.h"
#include "inviwo/core/io/ivfbasesettings.h"
#include "inviwo/core/io/volumereader.h"

namespace inviwo {


class IVW_CORE_API ReaderSettings : public IvwSerializable { 
public:
    ReaderSettings() {}
    ReaderSettings(std::string rawFile, ivec3 resolution=ivec3(0), std::string format="UCHAR");
    ~ReaderSettings() {};

    //serialization
    virtual void serialize(IvwSerializer& s) const {
        s.serialize("RawFileAbsolutePath", rawFileAbsolutePath_);
        s.serialize("DataFormat", dataFormat_);
        s.serialize("Dimensions", dimensions_);        
    }

    //de-serialization
    virtual void deserialize(IvwDeserializer& d) {
        d.deserialize("RawFileAbsolutePath", rawFileAbsolutePath_);
        d.deserialize("DataFormat", dataFormat_);
        d.deserialize("Dimensions", dimensions_);
    }

    //member variables
    std::string rawFileAbsolutePath_;
    uvec3 dimensions_;
    std::string dataFormat_; 
};


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
