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

#ifndef IVW_LAYER_H
#define IVW_LAYER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/data.h>
#include <inviwo/core/datastructures/spatialdata.h>

namespace inviwo {

class LayerRepresentation;

class IVW_CORE_API Layer : public Data, public StructuredGridMetaData<2> {
public:
    Layer(uvec2 dimensions = uvec2(256,256), const DataFormatBase* format = DataVec4UINT8::get());
    Layer(LayerRepresentation*);
    Layer(const Layer&);
    virtual ~Layer();
    void resize(uvec2 dimensions);
    virtual Layer* clone() const;
    void resizeLayerRepresentations(Layer* targetLayer, uvec2 targetDim);

	uvec2 getDimension() const;
	void setDimension(const uvec2& dim);

protected:
    virtual DataRepresentation* createDefaultRepresentation();

};

} // namespace

#endif // IVW_LAYER_H
