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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_RAWVOLUMEWRITER_H
#define IVW_RAWVOLUMEWRITER_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/common/inviwo.h"
#include "inviwo/core/io/volumewriter.h"

namespace inviwo {

class IVW_CORE_API RawVolumeWriter {
public:
    RawVolumeWriter();
    virtual ~RawVolumeWriter() {}

   static  void saveRawData(WriterSettings& writerSettings);

protected:
    template <class T>
    static void saveData(std::string rawFileAbsolutePath, uvec3 dimensions, const void* texels) {        
        std::fstream fout(rawFileAbsolutePath.c_str(), std::ios::out | std::ios::binary);
        ivwAssert(fout.good(), "cannot write volume file");        
        fout.write((char*)texels, dimensions.x*dimensions.y*dimensions.z*sizeof(T));
        fout.close();        
    }

};

} // namespace

#endif // IVW_RAWVOLUMEWRITER_H
