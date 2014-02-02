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

#include <inviwo/core/ports/volumeport.h>


namespace inviwo {

uvec3 VolumeInport::colorCode = uvec3(188,101,101);

// Volume Inport
VolumeInport::VolumeInport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel)
    : DataInport<Volume>(identifier, invalidationLevel)
{}

VolumeInport::~VolumeInport() {
}

void VolumeInport::initialize() {}

void VolumeInport::deinitialize() {}


uvec3 VolumeInport::getColorCode() const {
    return VolumeInport::colorCode;
}

// Volume Outport
VolumeOutport::VolumeOutport(std::string identifier, PropertyOwner::InvalidationLevel invalidationLevel)
    : DataOutport<Volume>(identifier, invalidationLevel)
{
}

VolumeOutport::~VolumeOutport() {}

void VolumeOutport::initialize() {}

void VolumeOutport::deinitialize() {}

uvec3 VolumeOutport::getColorCode() const {
    return VolumeInport::colorCode;
}

} // namespace