/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_VOLUMESOURCE_H
#define IVW_VOLUMESOURCE_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/ports/volumeport.h>

namespace inviwo {

class IVW_MODULE_BASE_API VolumeSource : public Processor {
public:
    VolumeSource();
    ~VolumeSource();

    InviwoProcessorInfo();

protected:
    void loadVolume();

private:
    VolumeOutport volumePort_;
    FileProperty volumeFile_;

    IntVec2Property dataRange_;
    FloatVec2Property valueRange_;
    StringProperty valueUnit_;

    void invalidateOutput();
    void updateRangeProperties(Volume* volume);
};

} // namespace

#endif // IVW_VOLUMESOURCE_H
