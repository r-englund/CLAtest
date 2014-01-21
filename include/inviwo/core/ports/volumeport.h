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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_VOLUMEPORT_H
#define IVW_VOLUMEPORT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/ports/datainport.h>
#include <inviwo/core/ports/dataoutport.h>
#include <inviwo/core/datastructures/volume/volume.h>

namespace inviwo {

class IVW_CORE_API VolumeInport : public DataInport<Volume> {

public:
    VolumeInport(std::string identifier,
                 PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~VolumeInport();

    void initialize();
    void deinitialize();

    uvec3 getColorCode() const;
    static uvec3 colorCode;
    virtual std::string getClassName() const {return "VolumeInport";}
};

class IVW_CORE_API VolumeOutport : public DataOutport<Volume> {

public:
    VolumeOutport(std::string identifier,
                  PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT);
    virtual ~VolumeOutport();

    void initialize();
    void deinitialize();

    uvec3 getColorCode() const;
    virtual std::string getClassName() const {return "VolumeOutport";}
};

} // namespace

#endif // IVW_VOLUMEPORT_H
