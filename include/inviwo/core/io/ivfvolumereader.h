#ifndef IVW_IVFVOLUMEREADER_H
#define IVW_IVFVOLUMEREADER_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/common/inviwo.h"
#include "inviwo/core/io/volumereader.h"
#include "inviwo/core/io/rawvolumereader.h"
#include "inviwo/core/util/filedirectory.h"

namespace inviwo {

class IVW_CORE_API IvfReaderSettings : public ReaderSettings, public IvwSerializable {
public:
    IvfReaderSettings();

    //get functions
    std::string getRawFileAbsolutePath() {return rawFileAbsolutePath_;}
    std::string getDataFormat() {return dataFormat_;}
    uvec3 getDimensions() {return dimensions_;}
    vec3 getSliceThickness() {return sliceThickness_;}
    float getTimeStep() {return timeStep_;}
    std::string getUnit() {return unit_;}
    std::string getModality() {return modality_;}
    mat4 getTransformationMatrix() {return transformationMatrix_;}

    //set functions
    void setRawFileAbsolutePath(const std::string &absolutePath) { rawFileAbsolutePath_ = absolutePath;}
    void setDataFormat(const std::string& format) {dataFormat_ = format;}
    void setDimensions(const uvec3& dimensions) {dimensions_ = dimensions;}
    void setSliceThickness(const vec3& thickness) {sliceThickness_ = thickness;}
    void setTimeStep(const float& timeStep) {timeStep_ = timeStep;}
    void setUnit(const std::string& unit) {unit_ = unit;}
    void setModality(const std::string& modality) {modality_ = modality;}
    void setTransformationMatrix(const mat4& tfMat) {transformationMatrix_ = tfMat;}


    //serialization
    virtual void serialize(IvwSerializer& s) const;

    //de-serialization
    virtual void deserialize(IvwDeserializer& d);
private:
    std::vector<std::string> xmlTags_;

    vec3 sliceThickness_;
    float timeStep_;
    std::string unit_;
    std::string modality_;
    mat4 transformationMatrix_;
};

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
        }       
    }

private:               
    std::string sourceFileAbsolutePath_; //Absolute path to the file
    Data* readRawVolumeData();
};    

} // namespace

#endif // IVW_IVFVOLUMEREADER_H
