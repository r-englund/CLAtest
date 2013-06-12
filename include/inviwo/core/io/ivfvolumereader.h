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

    IvfVolumeReader(const std::string filePath);

    virtual ~IvfVolumeReader() {}

    virtual Data* readData();
    
    //RawVolumeReader::ReaderSettings& getReaderSettings() const;

    static void readIvfFileSettings(const std::string filePath, IvfReaderSettings& ivfReaderSettings)  {
        ivfReaderSettings.rawFileAbsolutePath_ = "";
        ivfReaderSettings.dataFormat_ = "";
        ivfReaderSettings.dimensions_ = ivec3(0,0,0);
       
        std::string fileDirectory = UrlParser::getFileDirectory(filePath);
        
        std::string fileExtension = UrlParser::getFileExtension(filePath);

        ivwAssert(fileExtension=="ivf", "should be a *.ivf file");

        if (fileExtension=="ivf") {
            
            //Read the ivf file content
            IvwDeserializer d(filePath);
            ivfReaderSettings.deserialize(d);

            //translate
            ivfReaderSettings.rawFileAbsolutePath_ = fileDirectory + ivfReaderSettings.rawFileAbsolutePath_ ;
            if (ivfReaderSettings.dataFormat_=="UCHAR") {
                ivfReaderSettings.dataFormat_ = DataUINT8::str();
            }
            else if (ivfReaderSettings.dataFormat_=="USHORT") {
                ivfReaderSettings.dataFormat_ = DataUINT16::str();
            }
            else
                ivfReaderSettings.dataFormat_="";
        }        
    }

private:               
    std::string sourceFileAbsolutePath_; //Absolute path to the file
    Data* readRawVolumeData();
};    

} // namespace

#endif // IVW_IVFVOLUMEREADER_H
