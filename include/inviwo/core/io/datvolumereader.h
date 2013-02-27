#ifndef IVW_DATVOLUMEREADER_H
#define IVW_DATVOLUMEREADER_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/inviwo.h"
#include "inviwo/core/io/volumereader.h"
#include "inviwo/core/io/rawvolumereader.h"


namespace inviwo {

    
//Move this to util
class IVW_CORE_API UrlParser {
public:
    UrlParser() {}

    static std::string getFileDirectory(const std::string url) {
        size_t pos = url.find_last_of("\\/") + 1;
        std::string fileDirectory = url.substr(0, pos);
        return fileDirectory;
    }

    static std::string getFileExtension(const std::string url) {
        size_t pos = url.rfind(".") + 1;
        std::string fileExtension = url.substr( pos, url.length());
        return fileExtension;
    }
};

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
