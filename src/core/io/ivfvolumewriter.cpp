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

#include <inviwo/core/io/ivfvolumewriter.h>
#include <inviwo/core/util/filedirectory.h>
#include <inviwo/core/datastructures/volume/volumeram.h>

namespace inviwo {

IvfVolumeWriter::IvfVolumeWriter() : DataWriterType<Volume>(){
     addExtension(FileExtension("ivf","Inviwo ivf file format"));
}

IvfVolumeWriter::IvfVolumeWriter( const IvfVolumeWriter& rhs ) : DataWriterType<Volume>(rhs){
}

IvfVolumeWriter& IvfVolumeWriter::operator=( const IvfVolumeWriter& that ){
    if (this != &that) {
        DataWriterType<Volume>::operator=(that);
    }
    return *this;
}

IvfVolumeWriter* IvfVolumeWriter::clone() const{
    return new IvfVolumeWriter(*this);
}

void IvfVolumeWriter::writeData( const Volume* data, const std::string filePath ) const{

    std::string rawPath = URLParser::replaceFileExtension(filePath, "raw");

    if(URLParser::fileExists(filePath) && !overwrite_){
        throw DataWriterException("Error: Output file: " + filePath + " already exists");
    }
    if(URLParser::fileExists(rawPath) && !overwrite_){
        throw DataWriterException("Error: Output file: " + rawPath + " already exists");
    }

    std::string fileDirectory = URLParser::getFileDirectory(filePath);
    std::string fileExtension = URLParser::getFileExtension(filePath);
    std::string fileName = URLParser::getFileNameWithoutExtension(filePath);

    const VolumeRAM* vr = data->getRepresentation<VolumeRAM>();

    IvwSerializer s(filePath);

    s.serialize("ObjectFileName", fileName + ".raw");
    s.serialize("Format", vr->getDataFormatString());

    MetaDataMap* meta = data->getMetaDataMap();
    meta->add("string", new StringMetaData("hej"));
    meta->add("vec3", new Vec3MetaData(glm::vec3(1.0f)));
    meta->add("float", new FloatMetaData(3.f));
    meta->serialize(s);
    s.writeFile();

    std::fstream fout(rawPath.c_str(), std::ios::out | std::ios::binary);
    if(fout.good()){
        fout.write((char*)vr->getData(), 
            vr->getDimensions().x*vr->getDimensions().x*vr->getDimensions().x
            * vr->getDataFormat()->getBytesStored());
    }else{
        throw DataWriterException("Error: Could not write to raw file: " + rawPath);
    }

    fout.close();  
}

} // namespace
