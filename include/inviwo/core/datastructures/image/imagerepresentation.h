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

#ifndef IVW_IMAGEREPRESENTATION_H
#define IVW_IMAGEREPRESENTATION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/datarepresentation.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/imagetypes.h>
#include <inviwo/core/datastructures/image/image.h>

namespace inviwo {

class IVW_CORE_API ImageRepresentation : public DataRepresentation {

friend class Image;

public:
    ImageRepresentation(uvec2 dimensions, ImageType type, const DataFormatBase* format);
    virtual ~ImageRepresentation();
    virtual void performOperation(DataOperation*) const {};
    virtual void resize(uvec2 dimensions);
    const uvec2& getDimensions() const {return dimensions_;}
    // Removes old data and reallocate for new dimension.
    // Needs to be overloaded by child classes.
    virtual void setDimensions(uvec2 dimensions) { dimensions_ = dimensions;}
    virtual bool copyAndResizeImage(DataRepresentation*) = 0;
    virtual DataRepresentation* clone() const = 0;
    virtual std::string getClassName() const { return "ImageRepresentation"; }
    ImageType getImageType() const { return imageType_; }
protected:
    virtual void useInputSource(ImageLayerType, const Image*) {}
    virtual void createAndAddLayer(ImageLayerType) {}

    uvec2 dimensions_;
    ImageType imageType_;
};

} // namespace

#endif // IVW_IMAGEREPRESENTATION_H
