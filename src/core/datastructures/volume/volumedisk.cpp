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

#include <inviwo/core/datastructures/volume/volumedisk.h>

namespace inviwo {

VolumeDisk::VolumeDisk(uvec3 dimensions, const DataFormatBase* format)
    : VolumeRepresentation(dimensions, format), DiskRepresentation(){
}

VolumeDisk::VolumeDisk(std::string srcFile, uvec3 dimensions, const DataFormatBase* format)
    : VolumeRepresentation(dimensions, format), DiskRepresentation(srcFile){
    initialize();
}

VolumeDisk::VolumeDisk(const VolumeDisk& rhs) 
    : VolumeRepresentation(rhs)
    , DiskRepresentation(rhs) {    
}

VolumeDisk& VolumeDisk::operator=(const VolumeDisk& that) {
    if(this != &that) {
        VolumeRepresentation::operator=(that);
        DiskRepresentation::operator=(that);
    }
    return *this;
}

VolumeDisk* VolumeDisk::clone() const {
    return new VolumeDisk(*this);
}

VolumeDisk::~VolumeDisk() {}

void VolumeDisk::initialize() {}
void VolumeDisk::deinitialize() {}




} // namespace
