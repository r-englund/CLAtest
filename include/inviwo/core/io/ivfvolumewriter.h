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

#ifndef IVW_IVFVOLUMEWRITER_H
#define IVW_IVFVOLUMEWRITER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/io/datawriter.h>
#include <inviwo/core/datastructures/volume/volume.h>

namespace inviwo {

class IVW_CORE_API IvfVolumeWriter : public DataWriterType<Volume> {
public:        
    IvfVolumeWriter();
    IvfVolumeWriter(const IvfVolumeWriter& rhs);
    IvfVolumeWriter& operator=(const IvfVolumeWriter& that);
    virtual IvfVolumeWriter* clone() const;
    virtual ~IvfVolumeWriter() {}

    virtual void writeData(const Volume* data, const std::string filePath) const;
};  

} // namespace

#endif // IVW_IVFVOLUMEWRITER_H
