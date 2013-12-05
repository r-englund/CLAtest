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

namespace inviwo {

IvfVolumeReader::IvfVolumeReader() 
    : DataReaderType<Volume>()
    , format_(NULL) {
        addExtension(FileExtension("ivf", "Inviwo ivf file format"));
}

IvfVolumeReader::IvfVolumeReader( const IvfVolumeReader& rhs ) 
    : DataReaderType<Volume>(rhs)
    , meta_(rhs.meta_)
    , format_(rhs.format_){
}

IvfVolumeReader& IvfVolumeReader::operator=( const IvfVolumeReader& that ){
    if (this != &that) {
        meta_ = that.meta_;
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
    format_ = DataFormatBase::get(meta_.dataFormat_);
 
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
    if(fin.good()){
        std::size_t size = meta_.dimensions_.x*meta_.dimensions_.y*meta_.dimensions_.z*(format_->getBytesStored());
        fin.read((char*)destination, size);
    }else{
        throw DataReaderException("Error: Could not read from raw file: " + meta_.rawFileAbsolutePath_);
    }
    fin.close();
}

void* IvfVolumeReader::readData() const{
    std::size_t size = meta_.dimensions_.x*meta_.dimensions_.y*meta_.dimensions_.z*(format_->getBytesStored());
    char* data = new char[size];
    if(data){
        readDataInto(data);
    }else{
        throw DataReaderException("Error: Could not allocate memory for loading raw file: " + meta_.rawFileAbsolutePath_);
    }  
    return data;
}


} // namespace
