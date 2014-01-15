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
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#ifndef IVW_VOLUMEDISK_H
#define IVW_VOLUMEDISK_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/diskrepresentation.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>

namespace inviwo {

class IVW_CORE_API VolumeDisk : public VolumeRepresentation, public DiskRepresentation {

public:
    VolumeDisk(uvec3 dimensions = uvec3(128,128,128), const DataFormatBase* format = DataUINT8::get());
    VolumeDisk(std::string url, uvec3 dimensions = uvec3(128,128,128), const DataFormatBase* format = DataUINT8::get());
    VolumeDisk(const VolumeDisk& rhs);
    VolumeDisk& operator=(const VolumeDisk& that);
    virtual VolumeDisk* clone() const;
    virtual ~VolumeDisk();

    virtual void initialize();
    virtual void deinitialize();
};

} // namespace

#endif // IVW_VOLUMEDISK_H
