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

#ifndef IVW_LAYERRAM_H
#define IVW_LAYERRAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/image/layerrepresentation.h>
#include <inviwo/core/util/formats.h>

namespace inviwo {

class IVW_CORE_API LayerRAM : public LayerRepresentation {

public:
    LayerRAM(uvec2 dimension = uvec2(256,256), LayerType type = COLOR_LAYER, const DataFormatBase* format = DataVec4UINT8::get());
    LayerRAM(const LayerRAM& rhs);
    LayerRAM& operator=(const LayerRAM& that);
    LayerRAM* clone() const = 0;
    virtual ~LayerRAM();

    virtual void initialize() = 0;
    virtual void deinitialize() = 0;

    virtual std::string getClassName() const;
    virtual void setDimension(uvec2 dimensions);
    virtual void resize(uvec2 dimensions) = 0;
    virtual bool copyAndResizeLayer(DataRepresentation*) const;

    void* getData();
    const void* getData() const;

    // Takes ownership of data pointer
    void setData(void* data);

    virtual void setValueFromSingleFloat(const uvec2& pos, float val) = 0;
    virtual void setValueFromVec2Float(const uvec2& pos, vec2 val) = 0;
    virtual void setValueFromVec3Float(const uvec2& pos, vec3 val) = 0;
    virtual void setValueFromVec4Float(const uvec2& pos, vec4 val) = 0;

    virtual float getValueAsSingleFloat(const uvec2& pos) const = 0;
    virtual vec2 getValueAsVec2Float(const uvec2& pos) const = 0;
    virtual vec3 getValueAsVec3Float(const uvec2& pos) const = 0;
    virtual vec4 getValueAsVec4Float(const uvec2& pos) const = 0;

    static inline unsigned int posToIndex(const uvec2& pos, const uvec2& dim) {
        return pos.x+(pos.y*dim.x);
    }

protected:
    void* data_;
};

/**
 * Factory for layers.
 * Creates an LayerRAM with data type specified by format.
 *
 * @param dimension of layer to create.
 * @param format of layer to create.
 * @return NULL if no valid format was specified.
 */
IVW_CORE_API LayerRAM* createLayerRAM(const uvec2& dimension, LayerType type, const DataFormatBase* format);

} // namespace

#endif // IVW_LAYERRAM_H
