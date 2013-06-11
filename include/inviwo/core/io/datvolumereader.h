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

    static void readDatFileSettings(const std::string filePath, ReaderSettings& readerSettings)  {
        readerSettings.rawFileAbsolutePath_ = "";
        readerSettings.dataFormat_ = "";
        readerSettings.dimensions_ = ivec3(0,0,0);
       
        std::string fileDirectory = UrlParser::getFileDirectory(filePath);
        
        std::string fileExtension = UrlParser::getFileExtension(filePath);

        ivwAssert(fileExtension=="dat", "should be a *.dat file");

        if (fileExtension=="dat") {
            
            //Read the dat file content
            std::istream* f = new std::ifstream(filePath.c_str());
            std::string textLine; 
            
            std::string key;
            while (!f->eof()) {
                getline(*f, textLine);
                std::stringstream ss(textLine);
                transform(textLine.begin(), textLine.end(), textLine.begin(), (int (*)(int))tolower);
                key = "";
                ss >> key;            
                if (key=="ObjectFileName:") {
                    ss >> readerSettings.rawFileAbsolutePath_;
                    readerSettings.rawFileAbsolutePath_ = fileDirectory + readerSettings.rawFileAbsolutePath_;
                }
                else if (key=="Resolution:") {
                    ss >> readerSettings.dimensions_.x;
                    ss >> readerSettings.dimensions_.y;
                    ss >> readerSettings.dimensions_.z;
                }
                else if (key=="Format:") {
                    ss >> readerSettings.dataFormat_;

                    if (readerSettings.dataFormat_=="UCHAR") {
                        readerSettings.dataFormat_ = DataUINT8::str();
                    }
                    else if (readerSettings.dataFormat_=="USHORT") {
                        readerSettings.dataFormat_ = DataUINT16::str();
                    }
                    else
                        readerSettings.dataFormat_="";
                }                    
            };
        }       
    }

private:               
    std::string sourceFileAbsolutePath_; //Absolute path to the file
    Data* readRawVolumeData();
};    

} // namespace

#endif // IVW_DATVOLUMEREADER_H
