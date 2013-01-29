#ifndef IVW_VOLUMEREADER_H
#define IVW_VOLUMEREADER_H

#include "inviwo/core/io/datareader.h"
#include "inviwo/core/metadata/metadatamap.h"

namespace inviwo {

class IVW_CORE_API VolumeReader : public DataReader {

public:
    VolumeReader();
    virtual ~VolumeReader() {}

    //virtual Data* readData(const std::string filePath)=0;
    virtual Data* readData()=0;
protected:
};

} // namespace

#endif // IVW_DATAREADER_H
