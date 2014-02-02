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
#include <inviwo/core/datastructures/datagroup.h>
#include <inviwo/core/datastructures/spatialdata.h>
#include <inviwo/core/datastructures/image/layer.h>
#include <inviwo/core/datastructures/image/imagetypes.h>

namespace inviwo {

class ImageRepresentation;

class IVW_CORE_API Image : public DataGroup, public StructuredGridEntity<2> {
public:
    Image(uvec2 dimensions = uvec2(256,256), ImageType type = COLOR_DEPTH, const DataFormatBase* format = DataVec4UINT8::get(),
          bool allowMissingLayers = false);
    Image(const Image&);
    Image& operator=(const Image& that);
    virtual Image* clone() const;
    virtual ~Image();

    void deinitialize();

    void initialize(const DataFormatBase*);

    uvec2 getDimension() const;

    size_t addColorLayer(Layer*);

    const std::vector<const Layer*>* getAllLayers() const;
    const std::vector<Layer*>* getAllLayers();

    const Layer* getLayer(LayerType, size_t idx = 0) const;
    Layer* getLayer(LayerType, size_t idx = 0);

    const Layer* getColorLayer(size_t idx = 0) const;
    Layer* getColorLayer(size_t idx = 0);

    size_t getNumberOfColorLayers() const;

    const Layer* getDepthLayer() const;
    Layer* getDepthLayer();

    const Layer* getPickingLayer() const;
    Layer* getPickingLayer();

    void resize(uvec2 dimensions);
    void resizeRepresentations(Image* targetImage, uvec2 targetDim);

    ImageType getImageType() const;
    const DataFormatBase* getDataFormat() const;
    void setInputSource(LayerType, const Image*);

protected:
    void addLayer(Layer*);

    void setDimension(const uvec2& dim);

    std::vector<Layer*> colorLayers_;
    Layer* depthLayer_;
    Layer* pickingLayer_;

    std::vector<Layer*> allLayers_;
    std::vector<const Layer*> allLayersConst_;

private:
    bool allowMissingLayers_;
    ImageType imageType_;
    typedef std::map<LayerType, const Image*> ImageSourceMap;
    ImageSourceMap inputSources_;
};

} // namespace

#endif // IVW_IMAGE_H
