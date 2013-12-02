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

#include <inviwo/core/io/datvolumereader.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>
#include <inviwo/core/datastructures/volume/volumeramprecision.h>
#include <inviwo/core/datastructures/volume/volumetypeclassification.h>

namespace inviwo {

DatVolumeReader::DatVolumeReader() 
    : VolumeReader()
    , rawFile_("")
    , dimension_(uvec3(0,0,0))
    , format_(NULL) {
        addExtension(FileExtension("dat", "Inviwo dat file format"));
}

DatVolumeReader::DatVolumeReader( const DatVolumeReader& rhs ) 
    : VolumeReader(rhs)
    , rawFile_(rhs.rawFile_)
    , dimension_(rhs.dimension_)
    , format_(rhs.format_){};

DatVolumeReader& DatVolumeReader::operator=( const DatVolumeReader& that ){
    if (this != &that) {
        rawFile_ = that.rawFile_;
        dimension_ = that.dimension_;
        format_ = that.format_;
        VolumeReader::operator=(that);
    }
    return *this;
}

DatVolumeReader* DatVolumeReader::clone() const{
    return new DatVolumeReader(*this);
}

Volume* DatVolumeReader::readMetaData(std::string filePath)  {
    
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

    ivwAssert(fileExtension=="dat", "should be a *.dat file");

    //Read the dat file content
    std::istream* f = new std::ifstream(filePath.c_str());
    std::string textLine; 
    std::string formatFlag = "";

    Volume* volume = new UniformRectiLinearVolume();

    std::string key;
    while (!f->eof()) {
        getline(*f, textLine);
        std::stringstream ss(textLine);
        transform(textLine.begin(), textLine.end(), textLine.begin(), (int (*)(int))tolower);
        key = "";
        ss >> key;            
        if (key=="ObjectFileName:") {
            ss >> rawFile_;
            rawFile_ = fileDirectory + rawFile_;
        }else if (key=="Resolution:") {
            ss >> dimension_.x;
            ss >> dimension_.y;
            ss >> dimension_.z;
        }else if (key=="Format:") {
            ss >> formatFlag;
            format_ = DataFormatBase::get(formatFlag);
        }else{
            volume->setMetaData<StringMetaData>(key, textLine);
        }                    
    };

    volume->setDimension(dimension_);
    volume->setDataFormat(format_);
    VolumeDisk* vd = new VolumeDisk(filePath, dimension_, format_);
    vd->setDataReader(this);

    volume->addRepresentation(vd);

    return volume;
}    

void DatVolumeReader::readDataInto(void* destination) const {
    std::fstream fin(rawFile_.c_str(), std::ios::in | std::ios::binary);
    ivwAssert(fin.good(), "cannot open volume file");
    std::streamsize size = dimension_.x*dimension_.y*dimension_.z*(format_->getBytesStored());
    fin.read((char*)destination, size);
    fin.close();
}

void* DatVolumeReader::readData() const {
    std::streamsize size = dimension_.x*dimension_.y*dimension_.z*(format_->getBytesStored());
    char* data = new char[size];
    readDataInto(data);
    return data;
}



} // namespace
