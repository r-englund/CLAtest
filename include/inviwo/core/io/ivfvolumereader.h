#ifndef IVW_IVFVOLUMEREADER_H
#define IVW_IVFVOLUMEREADER_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/common/inviwo.h"
#include "inviwo/core/io/volumereader.h"
#include "inviwo/core/io/rawvolumereader.h"
#include "inviwo/core/util/filedirectory.h"
#include "inviwo/core/io/ivfreadersettings.h"

namespace inviwo {

class IVW_CORE_API IvfVolumeReader : public VolumeReader {
public:        

    IvfVolumeReader();
    IvfVolumeReader(const IvfVolumeReader& rhs);
    IvfVolumeReader& operator=(const IvfVolumeReader& that);
    virtual IvfVolumeReader* clone() const;
    virtual ~IvfVolumeReader() {}

    virtual Volume* readMetaData(const std::string filePath);
    virtual void* readData() const;
    virtual void readDataInto(void* dest) const;

private:
    IvfReaderSettings meta_;
    const DataFormatBase* format_;
};    

} // namespace

#endif // IVW_IVFVOLUMEREADER_H
