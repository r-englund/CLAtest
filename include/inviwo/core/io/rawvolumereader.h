#ifndef IVW_RAWVOLUMEREADER_H
#define IVW_RAWVOLUMEREADER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API RawVolumeReader : public DataReaderType<Volume> {
public:
    RawVolumeReader();
    RawVolumeReader(const RawVolumeReader& rhs);
    RawVolumeReader& operator=(const RawVolumeReader& that);
    virtual RawVolumeReader* clone() const;
    virtual ~RawVolumeReader() {}

    virtual Volume* readMetaData(const std::string filePath);
    virtual void* readData() const;
    virtual void readDataInto(void* dest) const;

private:
    std::string rawFile_;
    bool littleEndian_;
    glm::uvec3 dimension_;
    const DataFormatBase* format_;
};

} // namespace

#endif // IVW_RAWVOLUMEREADER_H

