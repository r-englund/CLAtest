#include <inviwo/core/io/ivfvolumereader.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>
#include "inviwo/core/datastructures/volume/volumedisk.h"

namespace inviwo {

IvfVolumeReader::IvfVolumeReader() 
    : VolumeReader()
    , format_(NULL) {
        addExtension(FileExtension("ivf", "Inviwo ivf file format"));
}

IvfVolumeReader::IvfVolumeReader( const IvfVolumeReader& rhs ) 
    : VolumeReader(rhs)
    , meta_(rhs.meta_)
    , format_(rhs.format_){
}

IvfVolumeReader& IvfVolumeReader::operator=( const IvfVolumeReader& that ){
    if (this != &that) {
        meta_ = that.meta_;
        format_ = that.format_;
        VolumeReader::operator=(that);
    }
    return *this;
}

IvfVolumeReader* IvfVolumeReader::clone() const{
    return new IvfVolumeReader(*this);
}

Volume* IvfVolumeReader::readMetaData(std::string filePath)  {   

    if(!URLParser::fileExists(filePath)){
        filePath = URLParser::addBasePath(filePath);
        if(!URLParser::fileExists(filePath)) {
            // FIXME: We need to throw an exception here (or return false)
            LogInfoCustom("DatVolumeReader::readMetaData", "File " + filePath + " does not exist.");     
            return NULL;
        }
    }

    std::string fileDirectory = URLParser::getFileDirectory(filePath);
    std::string fileExtension = URLParser::getFileExtension(filePath);

    ivwAssert(fileExtension=="ivf", "should be a *.ivf file");

    //Read the ivf file content
    IvwDeserializer d(filePath);
    meta_.deserialize(d);

    //translate
    meta_.rawFileAbsolutePath_ = fileDirectory + meta_.rawFileAbsolutePath_ ;
    if (meta_.dataFormat_=="UCHAR") {
        format_ = DataUINT8::get();
    }
    else if (meta_.dataFormat_=="USHORT") {
        format_ = DataUINT16::get();
    }
  
    Volume* volume = new UniformRectiLinearVolume();
    volume->setDimension(meta_.dimensions_);
    volume->setDataFormat(format_);
    VolumeDisk* vd = new VolumeDisk(meta_.rawFileAbsolutePath_, meta_.dimensions_, format_);
    vd->setDataReader(this);

    volume->addRepresentation(vd);

    return volume;
} 

void IvfVolumeReader::readDataInto(void* destination) const{
    std::fstream fin(meta_.rawFileAbsolutePath_.c_str(), std::ios::in | std::ios::binary);
    ivwAssert(fin.good(), "cannot open volume file");
    std::streamsize size = meta_.dimensions_.x*meta_.dimensions_.y*meta_.dimensions_.z*(format_->getBytesStored());
    fin.read((char*)destination, size);
    fin.close();
}

void* IvfVolumeReader::readData() const{
    std::streamsize size = meta_.dimensions_.x*meta_.dimensions_.y*meta_.dimensions_.z*(format_->getBytesStored());
    char* data = new char[size];
    readDataInto(data);
    return data;
}


} // namespace
