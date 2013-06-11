#ifndef IVW_VOLUMEREADER_H
#define IVW_VOLUMEREADER_H

#include "inviwo/core/io/datareader.h"
#include "inviwo/core/metadata/metadatamap.h"

namespace inviwo {

class IVW_CORE_API ReaderSettings { 
public:
    ReaderSettings() {}
    std::string rawFileAbsolutePath_;
    uvec3 dimensions_;
    std::string dataFormat_;
    ReaderSettings(std::string rawFile, ivec3 resolution=ivec3(0), std::string format="UCHAR");    
};

class IVW_CORE_API VolumeReader : public DataReader {

public:
    VolumeReader();
    virtual ~VolumeReader() {}

    //virtual Data* readData(const std::string filePath)=0;
    using DataReader::readData;
    virtual Data* readData()=0;
protected:
};

} // namespace

#endif // IVW_VOLUMEREADER_H
