#ifndef IVW_DATVOLUMEWRITER_H
#define IVW_DATVOLUMEWRITER_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/common/inviwo.h"
#include "inviwo/core/io/volumewriter.h"
#include "inviwo/core/util/filedirectory.h"

namespace inviwo {

class IVW_CORE_API DatVolumeWriter : public VolumeWriter {
public:        

    DatVolumeWriter(const std::string filePath, WriterSettings writerSettings);

    virtual ~DatVolumeWriter() {}

    virtual void writeData();

    static void writeDatFileSettings(const std::string filePath, WriterSettings& writerSettings)  {        
        //Write the dat file content
        //Note: Raw file writing need to be explicitly called.
        std::ofstream f(filePath.c_str());
        std::string textLine; 
        
        std::stringstream ss;
        std::string rawFileNameWithExtension = UrlParser::getFileNameWithExtension(writerSettings.rawFileAbsolutePath_);
        
        ss << "ObjectFileName: "<< rawFileNameWithExtension << std::endl;
        ss << "Resolution: ";
        ss << writerSettings.dimensions_.x << " ";
        ss << writerSettings.dimensions_.y << " ";
        ss << writerSettings.dimensions_.z << std::endl;

        std::string format="";
        if (writerSettings.dataFormat_== DataUINT8::str()) {
            format = "UCHAR";
        }
        else if (writerSettings.dataFormat_== DataUINT16::str()) {
            format = "USHORT";
        }

        ivwAssert(!format.empty(), "valid format required for writing");
        if (format.empty()) return;
        ss << "Format: " << format << std::endl;

        f << ss.str();        
        f.close();
        
    }

private:               
    std::string sourceFileAbsolutePath_; //Absolute path to the file
    WriterSettings writerFileSettings_; //Write file settings
    void writeRawVolumeData();
};    

} // namespace

#endif // IVW_DATVOLUMEWRITER_H
