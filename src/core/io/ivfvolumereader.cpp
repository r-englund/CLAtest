/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#include <inviwo/core/io/ivfvolumereader.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>
#include "inviwo/core/datastructures/volume/volumedisk.h"
#include "inviwo/core/util/filedirectory.h"

namespace inviwo {

IvfVolumeReader::IvfVolumeReader() 
    : DataReaderType<Volume>()
    , rawFile_("")
    , dimension_(uvec3(0))
    , format_(NULL) {
        addExtension(FileExtension("ivf", "Inviwo ivf file format"));
}

IvfVolumeReader::IvfVolumeReader( const IvfVolumeReader& rhs ) 
    : DataReaderType<Volume>(rhs)
    , rawFile_(rhs.rawFile_)
    , dimension_(rhs.dimension_)
    , format_(rhs.format_){
}

IvfVolumeReader& IvfVolumeReader::operator=( const IvfVolumeReader& that ){
    if (this != &that) {
        rawFile_ = that.rawFile_;
        dimension_ = that.dimension_;
        format_ = that.format_;
        DataReaderType<Volume>::operator=(that);
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
            throw DataReaderException("Error: Input file: " + filePath + " does not exist");
        }
    }

    std::string fileDirectory = URLParser::getFileDirectory(filePath);
    std::string fileExtension = URLParser::getFileExtension(filePath);

    Volume* volume = new UniformRectiLinearVolume();

    IvwDeserializer d(filePath);

    d.deserialize("ObjectFileName", rawFile_);
    rawFile_ = fileDirectory + rawFile_;
    std::string formatFlag("");
    d.deserialize("Format", formatFlag);
    format_ = DataFormatBase::get(formatFlag);

    volume->getMetaDataMap()->deserialize(d);
    dimension_ = volume->getDimension();
    
    VolumeDisk* vd = new VolumeDisk(filePath, dimension_, format_);
    vd->setDataReader(this);

    volume->addRepresentation(vd);

    return volume;
} 

void IvfVolumeReader::readDataInto(void* destination) const{
    std::fstream fin(rawFile_.c_str(), std::ios::in | std::ios::binary);
    if(fin.good()){
        std::size_t size = dimension_.x*dimension_.y*dimension_.z*(format_->getBytesStored());
        fin.read((char*)destination, size);
    }else{
        throw DataReaderException("Error: Could not read from raw file: " + rawFile_);
    }
    fin.close();
}

void* IvfVolumeReader::readData() const{
    std::size_t size = dimension_.x*dimension_.y*dimension_.z*(format_->getBytesStored());
    char* data = new char[size];
    if(data){
        readDataInto(data);
    }else{
        throw DataReaderException("Error: Could not allocate memory for loading raw file: " + rawFile_);
    }  
    return data;
}


} // namespace
