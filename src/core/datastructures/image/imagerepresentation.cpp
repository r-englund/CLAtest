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

ImageRepresentation::ImageRepresentation(uvec2 dimensions, ImageType type, const DataFormatBase* format)
    : DataRepresentation(format), dimensions_(dimensions), imageType_(type){
}

ImageRepresentation::~ImageRepresentation() {}

void ImageRepresentation::resize(uvec2 dimensions){
    dimensions_ = dimensions;
}    


} // namespace
