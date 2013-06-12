#ifndef IVW_IVFVOLUMEWRITER_H
#define IVW_IVFVOLUMEWRITER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/volumewriter.h>
#include <inviwo/core/io/ivfwritersettings.h>

namespace inviwo {

class IVW_CORE_API IvfVolumeWriter : public VolumeWriter {
public:        

    IvfVolumeWriter(const std::string filePath, IvfWriterSettings writerSettings);

    virtual ~IvfVolumeWriter() {}

    virtual void writeData();

    static void writeIvfFileSettings(const std::string filePath, IvfWriterSettings& ivfWriterSettings)  {        
        //Write the ivf file content
        //Note: Raw file writing need to be explicitly called.       

        std::string rawFileAbsolutePath = ivfWriterSettings.rawFileAbsolutePath_;

        //for serialization absolute path is not needed, so replace absolute path
        ivfWriterSettings.rawFileAbsolutePath_ = UrlParser::getFileNameWithExtension(ivfWriterSettings.rawFileAbsolutePath_);        

        std::string fileExtension = UrlParser::getFileExtension(filePath);

        ivwAssert(fileExtension=="ivf", "should be a *.ivf file");

        if (fileExtension=="ivf") {

            std::string format=ivfWriterSettings.dataFormat_;
            if (ivfWriterSettings.dataFormat_== DataUINT8::str()) {
                ivfWriterSettings.dataFormat_ = "UCHAR";
            }
            else if (ivfWriterSettings.dataFormat_== DataUINT16::str()) {
                ivfWriterSettings.dataFormat_ = "USHORT";
            }        
           

            //Read the ivf file content
            IvwSerializer s(filePath);
            ivfWriterSettings.serialize(s);
            s.writeFile();

            //revert back absolute path which may be used by others
            ivfWriterSettings.rawFileAbsolutePath_ = rawFileAbsolutePath;
             ivfWriterSettings.dataFormat_ = format;
        }

        
    }

private:               
    std::string sourceFileAbsolutePath_; //Absolute path to the file
    IvfWriterSettings writerFileSettings_; //Write file settings
    void writeRawVolumeData();
};  
} // namespace

#endif // IVW_IVFVOLUMEWRITER_H
