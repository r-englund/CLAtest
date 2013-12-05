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

#ifndef IVW_IVFWRITERSETTINGS_H
#define IVW_IVFWRITERSETTINGS_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/common/inviwo.h"
#include "inviwo/core/io/ivfbasesettings.h"

namespace inviwo {

class WriterSettings : public IvwSerializable {            
public:
    WriterSettings() {}    
    WriterSettings(std::string rawFile, ivec3 resolution=ivec3(0), std::string format="UCHAR");
    ~WriterSettings() {}

    //serialization
    virtual void serialize(IvwSerializer& s) const {
        s.serialize("RawFileAbsolutePath", rawFileAbsolutePath_);
        s.serialize("DataFormat", dataFormat_);
        s.serialize("Dimensions", dimensions_);        
    }

    //de-serialization
    virtual void deserialize(IvwDeserializer& d) {
        d.deserialize("RawFileAbsolutePath", rawFileAbsolutePath_);
        d.deserialize("DataFormat", dataFormat_);
        d.deserialize("Dimensions", dimensions_);        
    }

    //member varialbles
    std::string rawFileAbsolutePath_;
    uvec3 dimensions_;
    std::string dataFormat_;
    const void* texels_;
};

class IVW_CORE_API IvfWriterSettings : public WriterSettings, public IvfBaseSettings {
public:
    IvfWriterSettings();
    ~IvfWriterSettings() {}

    //serialization
    virtual void serialize(IvwSerializer& s) const;

    //de-serialization
    virtual void deserialize(IvwDeserializer& d);
};

} // namespace

#endif // IVW_IVFWRITERSETTINGS_H
