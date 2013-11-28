#ifndef IVW_DATVOLUMEREADER_H
#define IVW_DATVOLUMEREADER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/volumereader.h>
#include <inviwo/core/io/rawvolumereader.h>
#include <inviwo/core/util/filedirectory.h>

namespace inviwo {

class IVW_CORE_API DatVolumeReader : public VolumeReader {
public:        
    DatVolumeReader();
    DatVolumeReader(const DatVolumeReader& rhs);
    DatVolumeReader& operator=(const DatVolumeReader& that);
    virtual DatVolumeReader* clone() const;
    virtual ~DatVolumeReader() {}

    virtual Volume* readMetaData(const std::string filePath);
    virtual void* readData() const;
    virtual void readDataInto(void* dest) const;

private:
    std::string rawFile_;
    glm::uvec3 dimension_;
    const DataFormatBase* format_;
};    

} // namespace

#endif // IVW_DATVOLUMEREADER_H
