#ifndef IVW_DATVOLUMEREADER_H
#define IVW_DATVOLUMEREADER_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/common/inviwo.h"
#include "inviwo/core/io/volumereader.h"
#include "inviwo/core/io/rawvolumereader.h"
#include "inviwo/core/util/filedirectory.h"

namespace inviwo {

class IVW_CORE_API DatVolumeReader : public VolumeReader {
public:        

    DatVolumeReader(const std::string filePath);

    virtual ~DatVolumeReader() {}

    virtual Data* readData();
    
    //RawVolumeReader::ReaderSettings& getReaderSettings() const;

    static void readDatFileSettings(std::string filePath, ReaderSettings& readerSettings);

private:               
    std::string sourceFileAbsolutePath_; //Absolute path to the file
    Data* readRawVolumeData();
};    

} // namespace

#endif // IVW_DATVOLUMEREADER_H
