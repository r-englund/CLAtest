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
#include <inviwo/core/util/urlparser.h>

namespace inviwo {

DatVolumeReader::DatVolumeReader() 
    : DataReaderType<Volume>()
    , rawFile_("")
    , littleEndian_(true)
    , dimension_(uvec3(0,0,0))
    , format_(NULL) {
        addExtension(FileExtension("dat", "Inviwo dat file format"));
}

DatVolumeReader::DatVolumeReader( const DatVolumeReader& rhs ) 
    : DataReaderType<Volume>(rhs)
    , rawFile_(rhs.rawFile_)
    , littleEndian_(true)
    , dimension_(rhs.dimension_)
    , format_(rhs.format_){};

DatVolumeReader& DatVolumeReader::operator=( const DatVolumeReader& that ){
    if (this != &that) {
        rawFile_ = that.rawFile_;
        littleEndian_ = that.littleEndian_;
        dimension_ = that.dimension_;
        format_ = that.format_;
        DataReaderType<Volume>::operator=(that);
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
            throw DataReaderException("Error: Input file: " + filePath + " does not exist");
        }
    }

    std::string fileDirectory = URLParser::getFileDirectory(filePath);
    std::string fileExtension = URLParser::getFileExtension(filePath);

    //Read the dat file content
    std::istream* f = new std::ifstream(filePath.c_str());
    std::string textLine; 
    std::string formatFlag = "";

    Volume* volume = new UniformRectiLinearVolume();

    glm::mat3 basis(2.0f);
    glm::vec3 offset(0.0f);
    glm::vec3 spacing(0.0f);
    glm::mat4 wtm(1.0f);
    glm::vec3 a(0.0f), b(0.0f), c(0.0f);

    std::string key;
    while (!f->eof()) {
        getline(*f, textLine);
        std::stringstream ss(textLine);
        key = "";
        ss >> key;
        if( key == "" || key[0] == '#' || key[0] == '/' )
            continue;

        transform(key.begin(), key.end(), key.begin(), (int (*)(int))tolower);
        key.erase(key.end()-1);
        
        if (key == "objectfilename" || key == "rawfile") {
            ss >> rawFile_;
            rawFile_ = fileDirectory + rawFile_;
        } else if(key == "byteorder") {
            std::string type;
            ss >> type;
            transform(type.begin(), type.end(), type.begin(), (int(*)(int))tolower);
            if(type == "bigendian") {
                littleEndian_ = false;
            }
        }else if (key == "resolution" || key == "dimension" ) {
            ss >> dimension_.x;
            ss >> dimension_.y;
            ss >> dimension_.z;
        }else if (key == "spacing" || key == "slicethickness") {
            ss >> spacing.x;
            ss >> spacing.y;
            ss >> spacing.z;
        }else if (key == "basisvector1") {
            ss >> a.x;
            ss >> a.y;
            ss >> a.z;
        }else if (key == "basisvector2") {
            ss >> b.x;
            ss >> b.y;
            ss >> b.z;
        }else if (key == "basisvector3") {
            ss >> c.x;
            ss >> c.y;
            ss >> c.z;
        }else if (key == "offset") {
            ss >> offset.x;
            ss >> offset.y;
            ss >> offset.z;
        }else if (key == "worldvector1") {
            ss >> wtm[0][0];
            ss >> wtm[1][0];
            ss >> wtm[2][0];
            ss >> wtm[3][0];
        }else if (key == "worldvector2") {
            ss >> wtm[0][1];
            ss >> wtm[1][1];
            ss >> wtm[2][1];
            ss >> wtm[3][1];
        }else if (key == "worldvector3") {
            ss >> wtm[0][2];
            ss >> wtm[1][2];
            ss >> wtm[2][2];
            ss >> wtm[3][2];
        }else if (key == "worldvector4") {
            ss >> wtm[0][3];
            ss >> wtm[1][3];
            ss >> wtm[2][3];
            ss >> wtm[3][3];
        }else if (key == "format") {
            ss >> formatFlag;
            format_ = DataFormatBase::get(formatFlag);
        }else{
            volume->setMetaData<StringMetaData>(key, ss.str());
        }                    
    };
    delete f;

    if(dimension_ == uvec3(0)){
        throw DataReaderException("Error: Unable to find \"Resolution\" tag in .dat file: " + filePath);
    }else if(format_ == NULL){
        throw DataReaderException("Error: Unable to find \"Format\" tag in .dat file: " + filePath);
    }else if(rawFile_ == ""){
        throw DataReaderException("Error: Unable to find \"ObjectFilename\" tag in .dat file: " + filePath);
    }

    if( spacing != vec3(0.0f) ) {
        basis[0][0] = dimension_.x * spacing.x;
        basis[1][1] = dimension_.y * spacing.y;
        basis[2][2] = dimension_.z * spacing.z;
    }
    if ( a!= vec3(0.0f) && b != vec3(0.0f) && c != vec3(0.0f) ){
        basis[0][0]=a.x;basis[1][0]=a.y;basis[2][0]=a.z;
        basis[0][1]=b.x;basis[1][1]=b.y;basis[2][1]=b.z;
        basis[0][2]=c.x;basis[1][2]=c.y;basis[2][2]=c.z;
    }
    // If not specified, center the data around origo.
    if( offset == vec3(0.0f)){
        offset[0] = -basis[0][0]/2.0f;
        offset[1] = -basis[1][1]/2.0f;
        offset[2] = -basis[2][2]/2.0f;
    }
    
    volume->setBasis(basis);
    volume->setOffset(offset);
    volume->setWorldTransform(wtm);
    volume->setDimension(dimension_);
    volume->setDataFormat(format_);
    VolumeDisk* vd = new VolumeDisk(filePath, dimension_, format_);
    vd->setDataReader(this);

    volume->addRepresentation(vd);

    return volume;
}    

void DatVolumeReader::readDataInto(void* destination) const {
    std::fstream fin(rawFile_.c_str(), std::ios::in | std::ios::binary);
    if(fin.good()){
        std::size_t size = dimension_.x*dimension_.y*dimension_.z*(format_->getBytesStored());
        fin.read(static_cast<char*>(destination), size);

        if(!littleEndian_ && format_->getBytesStored()>1) {
            std::size_t bytes = format_->getBytesStored();
            char* temp = new char[bytes];

            for(std::size_t i = 0; i < size; i += bytes) {
                for(std::size_t j = 0; j < bytes; j++) {
                    temp[j] = static_cast<char*>(destination)[i + j];
                }
                for(std::size_t j = 0; j < bytes; j++) {
                    static_cast<char*>(destination)[i + j] = temp[bytes - j - 1];
                }
            }
            delete temp;
        }
    }else{
        throw DataReaderException("Error: Could not read from raw file: " + rawFile_);
    }
    fin.close();
}

void* DatVolumeReader::readData() const {
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
