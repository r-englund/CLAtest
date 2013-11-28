#ifndef IVW_VOLUMEREADER_H
#define IVW_VOLUMEREADER_H

#include <inviwo/core/io/datareader.h>
#include <inviwo/core/metadata/metadatamap.h>
#include <inviwo/core/datastructures/volume/volume.h>

namespace inviwo {

class IVW_CORE_API VolumeReader : public DataReaderType<Volume>  {

public:
    VolumeReader();
    VolumeReader(const VolumeReader& rhs);
    VolumeReader& operator=(const VolumeReader& that);
    virtual VolumeReader* clone() const = 0;
    virtual ~VolumeReader() {}

    virtual Volume* readMetaData(const std::string filePath) = 0;
    virtual void* readData() const = 0;
    virtual void readDataInto(void* dest) const = 0;

protected:

};  

} // namespace

#endif // IVW_VOLUMEREADER_H
