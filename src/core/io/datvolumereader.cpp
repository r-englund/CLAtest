#include "inviwo/core/io/datvolumereader.h"
#include "inviwo/core/datastructures/volume/volumeramprecision.h"
#include "inviwo/core/datastructures/volume/volumetypeclassification.h"

namespace inviwo {


DatVolumeReader::DatVolumeReader(const std::string filePath) 
    : VolumeReader()
    , sourceFileAbsolutePath_(filePath)
{}

Data* DatVolumeReader::readData() {    
    return readRawVolumeData();
}

void DatVolumeReader::readDatFileSettings(std::string filePath, ReaderSettings& readerSettings)  {
    readerSettings.rawFileAbsolutePath_ = "";
    readerSettings.dataFormat_ = "";
    readerSettings.dimensions_ = ivec3(0,0,0);

    if(!UrlParser::fileExists(filePath)){
        filePath = UrlParser::addBasePath(filePath);
        if(!UrlParser::fileExists(filePath)) {
            ivwAssert(NULL, "File " + filePath + " does not exist.");     
            return;
        }
    }

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

Data* DatVolumeReader::readRawVolumeData() {

    ReaderSettings readerSettings;

    //Get reader settings
    DatVolumeReader::readDatFileSettings(sourceFileAbsolutePath_, readerSettings);    

    //read raw data
    void* rawData = RawVolumeReader::loadRawData(readerSettings);

    ivwAssert(rawData!=0, "Unable to read raw file data");        

    //Allocate volume handle
    Volume* data = new StandardVolume();
    DataRepresentation* dataRepresentation = 0;

    //Allocate representation
    if (readerSettings.dataFormat_ == "UINT8") {
        dataRepresentation = new VolumeRAMuint8(static_cast<uint8_t*>(rawData), readerSettings.dimensions_);            
    }
    else if (readerSettings.dataFormat_ == "UINT16") {
        dataRepresentation = new VolumeRAMuint16(static_cast<uint16_t*>(rawData), readerSettings.dimensions_);            
    }
    else {
        delete data;
        data = 0;
        return data;
    }

    //add representation to handle
    data->addRepresentation(dataRepresentation);
    return data;
}




} // namespace
