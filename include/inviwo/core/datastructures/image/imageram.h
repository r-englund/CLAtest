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

#ifndef IVW_IMAGERAM_H
#define IVW_IMAGERAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/image/imagerepresentation.h>
#include <inviwo/core/datastructures/image/layerram.h>
#include <inviwo/core/util/formats.h>

namespace inviwo {

class IVW_CORE_API ImageRAM : public ImageRepresentation {

public:
    ImageRAM();
    ImageRAM(const ImageRAM& rhs);
    virtual ~ImageRAM();
    DataRepresentation* clone() const;
    virtual void initialize();
    virtual void deinitialize();
    virtual std::string getClassName() const;

    virtual bool copyAndResizeImageRepresentation(ImageRepresentation*) const;

    LayerRAM* getColorLayerRAM(size_t idx = 0);
    LayerRAM* getDepthLayerRAM();
    LayerRAM* getPickingLayerRAM();

    const LayerRAM* getColorLayerRAM(size_t idx = 0) const;
    const LayerRAM* getDepthLayerRAM() const;
    const LayerRAM* getPickingLayerRAM() const;

protected:
    virtual void update(bool editable);

private:
    std::vector<LayerRAM*> colorLayersRAM_;
    LayerRAM* depthLayerRAM_;
    LayerRAM* pickingLayerRAM_;
};

} // namespace

#endif // IVW_IMAGERAM_H
