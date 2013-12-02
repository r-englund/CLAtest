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
#include "inviwo/core/io/volumewriter.h"

namespace inviwo {

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
