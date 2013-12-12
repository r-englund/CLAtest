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

#ifndef IVW_DATVOLUMEWRITER_H
#define IVW_DATVOLUMEWRITER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/datawriter.h>
#include <inviwo/core/datastructures/volume/volume.h>

namespace inviwo {

class IVW_CORE_API DatVolumeWriter : public DataWriterType<Volume> {
public:        
    DatVolumeWriter();
    DatVolumeWriter(const DatVolumeWriter& rhs);
    DatVolumeWriter& operator=(const DatVolumeWriter& that);
    virtual DatVolumeWriter* clone() const;
    virtual ~DatVolumeWriter() {};

    virtual void writeData(const Volume* data, const std::string filePath) const;

private:
    template<typename T>
    void writeKeyToString(std::stringstream& ss, const std::string& key, const glm::detail::tvec2<T>& vec) const;
    template<typename T>
    void writeKeyToString(std::stringstream& ss, const std::string& key, const glm::detail::tvec3<T>& vec) const;
    template<typename T>
    void writeKeyToString(std::stringstream& ss, const std::string& key, const glm::detail::tvec4<T>& vec) const;
    void writeKeyToString(std::stringstream& ss, const std::string& key, const std::string& str) const;
};

template<typename T>
void inviwo::DatVolumeWriter::writeKeyToString( std::stringstream& ss, const std::string& key, const glm::detail::tvec2<T>& vec ) const{
    ss << key << ": " << vec.x << " " << vec.y << std::endl;
}
template<typename T>
void inviwo::DatVolumeWriter::writeKeyToString( std::stringstream& ss, const std::string& key, const glm::detail::tvec3<T>& vec ) const{
    ss << key << ": " << vec.x << " " << vec.y << " " << vec.z << std::endl;
}
template<typename T>
void inviwo::DatVolumeWriter::writeKeyToString( std::stringstream& ss, const std::string& key, const glm::detail::tvec4<T>& vec ) const{
    ss << key << ": " << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << std::endl;
}





} // namespace

#endif // IVW_DATVOLUMEWRITER_H
