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

#ifndef IVW_IMAGE_H
#define IVW_IMAGE_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/spatialdata.h>
#include <inviwo/core/datastructures/image/imagetypes.h>

namespace inviwo {

class ImageRepresentation;

class IVW_CORE_API Image : public Data, public StructuredGridMetaData<2> {
public:
    Image(uvec2 dimensions = uvec2(256,256), ImageType type = COLOR_DEPTH, const DataFormatBase* format = DataVec4UINT8::get(), bool allowMissingLayers = false);
    Image(ImageRepresentation*, bool allowMissingLayers = false);
    Image(const Image&);
    virtual ~Image();
    void resize(uvec2 dimensions);
    virtual Image* clone() const;
    void resizeImageRepresentations(Image* targetImage, uvec2 targetDim);
    ImageType getImageType() const { return imageType_; }
    void setInputSource(ImageLayerType, const Image*);
    void setAllowMissingLayers(bool);

	uvec2 getDimension() const;
	void setDimension(const uvec2& dim);

protected:
    virtual DataRepresentation* createDefaultRepresentation();
    void newRepresentationCreated() const;
private:
    bool allowMissingLayers_;
    ImageType imageType_;
    typedef std::map<ImageLayerType, const Image*> ImageSourceMap;
    ImageSourceMap inputSources_;
};

} // namespace

#endif // IVW_IMAGE_H
