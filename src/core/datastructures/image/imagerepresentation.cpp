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

#include <inviwo/core/datastructures/image/imagerepresentation.h>

namespace inviwo {

ImageRepresentation::ImageRepresentation()
    : DataGroupRepresentation() {
}

ImageRepresentation::ImageRepresentation(const ImageRepresentation& rhs)
    : DataGroupRepresentation(rhs), owner_(rhs.owner_) {
}
ImageRepresentation& ImageRepresentation::operator=(const ImageRepresentation& that) {
    if (this != &that) {
        owner_ = that.owner_;
        DataGroupRepresentation::operator=(that);
    }

    return *this;
}

ImageRepresentation::~ImageRepresentation() {}

void ImageRepresentation::performOperation(DataOperation*) const {}

std::string ImageRepresentation::getClassName() const {
    return "ImageRepresentation";
}

uvec2 ImageRepresentation::getDimension() const {
    return owner_->getDimension();
}

const Image* ImageRepresentation::getOwner() const {
    return owner_;
}

void ImageRepresentation::setPointerToOwner(DataGroup* owner) {
    owner_ = dynamic_cast<Image*>(owner);
}



} // namespace
