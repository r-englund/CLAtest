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

#include <inviwo/core/datastructures/volume/volumerepresentation.h>

namespace inviwo {

    VolumeRepresentation::VolumeRepresentation(uvec3 dimension)
        : DataRepresentation(),
        dimensions_(dimension),
        borders_(VolumeBorders())
    {}

    VolumeRepresentation::VolumeRepresentation(uvec3 dimension, const DataFormatBase* format)
        : DataRepresentation(format),
        dimensions_(dimension),
        borders_(VolumeBorders())
    {}

    VolumeRepresentation::VolumeRepresentation(uvec3 dimension, const DataFormatBase* format, VolumeBorders border)
        : DataRepresentation(format),
        dimensions_(dimension),
        borders_(border)

    {}

    VolumeRepresentation::VolumeRepresentation(const VolumeRepresentation& rhs)
        : DataRepresentation() 
        , dimensions_(rhs.dimensions_)
        , borders_(rhs.borders_){
    }

    VolumeRepresentation& VolumeRepresentation::operator=(const VolumeRepresentation& that) {
        if(this != &that) {
            dimensions_ = that.dimensions_;
            borders_ = that.borders_;
            DataRepresentation::operator=(that);
        }
        return *this;
    }
    VolumeRepresentation* VolumeRepresentation::clone() const {
        return new VolumeRepresentation(*this);
    }
    VolumeRepresentation::~VolumeRepresentation() {}




} // namespace
