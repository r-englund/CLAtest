#ifndef IVW_VOLUMEREADER_H
#define IVW_VOLUMEREADER_H

#include "inviwo/core/io/datareader.h"
#include "inviwo/core/metadata/metadatamap.h"

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

class IVW_CORE_API VolumeReader : public DataReader  {

public:
    VolumeReader();
    virtual ~VolumeReader() {}

    using DataReader::readData;
    virtual Data* readData()=0;
protected:
};

} // namespace

#endif // IVW_VOLUMEREADER_H
