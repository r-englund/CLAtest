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

#ifndef IVW_IVFVOLUMEREADER_H
#define IVW_IVFVOLUMEREADER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/datareader.h>
#include <inviwo/core/datastructures/volume/volume.h>

namespace inviwo {

class IVW_CORE_API IvfVolumeReader : public DataReaderType<Volume> {
public:        

    IvfVolumeReader();
    IvfVolumeReader(const IvfVolumeReader& rhs);
    IvfVolumeReader& operator=(const IvfVolumeReader& that);
    virtual IvfVolumeReader* clone() const;
    virtual ~IvfVolumeReader() {}

    virtual Volume* readMetaData(const std::string filePath);
    virtual void* readData() const;
    virtual void readDataInto(void* dest) const;

private:
    std::string rawFile_;
    bool littleEndian_;
    glm::uvec3 dimension_;
    const DataFormatBase* format_;
};    

} // namespace

#endif // IVW_IVFVOLUMEREADER_H
