#ifndef IVW_VOLUMEWRITER_H
#define IVW_VOLUMEWRITER_H

#include "inviwo/core/io/datawriter.h"
#include "inviwo/core/metadata/metadatamap.h"

namespace inviwo {

class WriterSettings {            
public:
    WriterSettings() {}
    std::string rawFileAbsolutePath_;
    uvec3 dimensions_;
    std::string dataFormat_;
    const void* texels_;
    WriterSettings(std::string rawFile, ivec3 resolution=ivec3(0), std::string format="UCHAR");
};

class IVW_CORE_API VolumeWriter : public DataWriter {

public:
    VolumeWriter();
    virtual ~VolumeWriter() {}

    virtual void writeData()=0;
};

} // namespace

#endif // IVW_VOLUMEWRITER_H
