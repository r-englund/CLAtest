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

#ifndef IVW_DATVOLUMEREADER_H
#define IVW_DATVOLUMEREADER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/volumereader.h>
#include <inviwo/core/io/rawvolumereader.h>
#include <inviwo/core/util/filedirectory.h>

namespace inviwo {

class IVW_CORE_API DatVolumeReader : public VolumeReader {
public:        
    DatVolumeReader();
    DatVolumeReader(const DatVolumeReader& rhs);
    DatVolumeReader& operator=(const DatVolumeReader& that);
    virtual DatVolumeReader* clone() const;
    virtual ~DatVolumeReader() {}

    virtual Volume* readMetaData(const std::string filePath);
    virtual void* readData() const;
    virtual void readDataInto(void* dest) const;

private:
    std::string rawFile_;
    glm::uvec3 dimension_;
    const DataFormatBase* format_;
};    

} // namespace

#endif // IVW_DATVOLUMEREADER_H
