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

#include <inviwo/core/datastructures/image/layerrepresentation.h>

namespace inviwo {

LayerRepresentation::LayerRepresentation(uvec2 dimensions, LayerType type, const DataFormatBase* format)
    : DataRepresentation(format), dimensions_(dimensions), layerType_(type) {
}

LayerRepresentation::LayerRepresentation(const LayerRepresentation& rhs) 
    : DataRepresentation(rhs)
    , dimensions_(rhs.dimensions_)
    , layerType_(rhs.layerType_){
}
LayerRepresentation& LayerRepresentation::operator=(const LayerRepresentation& that) {
    if(this != &that) {
        dimensions_ = that.dimensions_;
        layerType_ = that.layerType_;
        DataRepresentation::operator=(that);
    }
    return *this;
}

LayerRepresentation::~LayerRepresentation() {}

void LayerRepresentation::resize(uvec2 dimensions){
    dimensions_ = dimensions;
}

uvec2 LayerRepresentation::getDimension() const { 
    return dimensions_; 
}

void LayerRepresentation::setDimension(uvec2 dimensions) { 
    dimensions_ = dimensions;
}

LayerType LayerRepresentation::getLayerType() const { 
    return layerType_; 
}



} // namespace
