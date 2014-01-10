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

#include <inviwo/core/io/datvolumewriter.h>
#include <inviwo/core/util/filedirectory.h>
#include <inviwo/core/datastructures/volume/volumeram.h>


namespace inviwo {

DatVolumeWriter::DatVolumeWriter() : DataWriterType<Volume>() {
    addExtension(FileExtension("dat","Inviwo dat file format"));
}

DatVolumeWriter::DatVolumeWriter(const DatVolumeWriter& rhs) : DataWriterType<Volume>(rhs) {
}

DatVolumeWriter& DatVolumeWriter::operator=( const DatVolumeWriter& that ){
    if (this != &that) {
        DataWriterType<Volume>::operator=(that);
    }
    return *this;
}

DatVolumeWriter* DatVolumeWriter::clone() const{
    return new DatVolumeWriter(*this);
}

void DatVolumeWriter::writeData(const Volume* data, const std::string filePath) const {

    std::string rawPath = URLParser::replaceFileExtension(filePath, "raw");

    if(URLParser::fileExists(filePath)  && !overwrite_ ){
        throw DataWriterException("Error: Output file: " + filePath + " already exists");
    }
    if(URLParser::fileExists(rawPath) && !overwrite_ ){
        throw DataWriterException("Error: Output file: " + rawPath + " already exists");
    }

    std::string fileDirectory = URLParser::getFileDirectory(filePath);
    std::string fileExtension = URLParser::getFileExtension(filePath);
    std::string fileName = URLParser::getFileNameWithoutExtension(filePath);

    //Write the .dat file content
    std::stringstream ss;

    const VolumeRAM* vr = data->getRepresentation<VolumeRAM>();
  
    glm::mat3 basis = glm::transpose(data->getBasis());
    glm::vec3 offset = data->getOffset();
    glm::mat4 wtm = glm::transpose(data->getWorldTransform());

    writeKeyToString(ss, "ObjectFileName",  fileName + ".raw");
    writeKeyToString(ss, "Resolution", vr->getDimension());
    writeKeyToString(ss, "Format",  vr->getDataFormatString());
    writeKeyToString(ss, "Offset", offset);
    writeKeyToString(ss, "BasisVector1", basis[0]);
    writeKeyToString(ss, "BasisVector2", basis[1]);
    writeKeyToString(ss, "BasisVector3", basis[2]);
    writeKeyToString(ss, "WorldVector1", wtm[0]);
    writeKeyToString(ss, "WorldVector2", wtm[1]);
    writeKeyToString(ss, "WorldVector3", wtm[2]);
    writeKeyToString(ss, "WorldVector4", wtm[3]);

    std::vector<std::string> keys = data->getMetaDataMap()->getKeys();

    for(std::vector<std::string>::iterator it = keys.begin();
        it != keys.end(); ++it){
        if(*it != "dimension" && *it != "basisAndOffset" && *it !="worldTransform"){
            MetaData* m = data->getMetaDataMap()->get(*it);

            StringMetaData* sm = dynamic_cast<StringMetaData*>(m);
            if(sm){
                writeKeyToString(ss, *it, sm->get());
            }
        }
    }

    std::ofstream f(filePath.c_str());
    if(f.good()){
        f << ss.str();
    }else{
        throw DataWriterException("Error: Could not write to dat file: " + filePath);
    }
    f.close();

    std::fstream fout(rawPath.c_str(), std::ios::out | std::ios::binary);
    if(fout.good()){
        fout.write((char*)vr->getData(), 
            vr->getDimension().x*vr->getDimension().x*vr->getDimension().x
            * vr->getDataFormat()->getBytesStored());
    }else{
          throw DataWriterException("Error: Could not write to raw file: " + rawPath);
    }

    fout.close();    
}

void DatVolumeWriter::writeKeyToString( std::stringstream& ss, const std::string& key, const std::string& str ) const{
    ss << key << ": " << str << std::endl;
}

} // namespace
