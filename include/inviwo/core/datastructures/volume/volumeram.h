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

#ifndef IVW_VOLUMERAM_H
#define IVW_VOLUMERAM_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/volume/volumerepresentation.h>
#include <inviwo/core/datastructures/histogram.h>

namespace inviwo {

class IVW_CORE_API VolumeRAM : public VolumeRepresentation {

public:
    VolumeRAM(uvec3 dimension = uvec3(128,128,128), VolumeRepresentation::VolumeBorders border = VolumeRepresentation::VolumeBorders(), const DataFormatBase* format = DataFormatBase::get());
    VolumeRAM(const VolumeRAM& rhs);
    VolumeRAM& operator=(const VolumeRAM& that);
    virtual VolumeRAM* clone() const = 0;
    virtual ~VolumeRAM();

    virtual void performOperation(DataOperation*) const = 0;
    virtual void initialize();
    virtual void deinitialize();

    virtual void* getData();
    virtual const void* getData() const;
    /** 
     * \brief Takes ownership of data pointer
     * 
     * @param void * data is raw volume data pointer
     * @return void none
     */
    void setData(void* data) {
        deinitialize();
        data_ = data;
    }

    bool hasNormalizedHistogram() const;
    NormalizedHistogram* getNormalizedHistogram();
    const NormalizedHistogram* getNormalizedHistogram() const;

    virtual void setValueFromSingleFloat(const uvec3& pos, float val) = 0;
    virtual void setValueFromVec2Float(const uvec3& pos, vec2 val) = 0;
    virtual void setValueFromVec3Float(const uvec3& pos, vec3 val) = 0;
    virtual void setValueFromVec4Float(const uvec3& pos, vec4 val) = 0;

    virtual float getValueAsSingleFloat(const uvec3& pos) const = 0;
    virtual vec2 getValueAsVec2Float(const uvec3& pos) const = 0;
    virtual vec3 getValueAsVec3Float(const uvec3& pos) const = 0;
    virtual vec4 getValueAsVec4Float(const uvec3& pos) const = 0;

    static inline unsigned int posToIndex(const uvec3& pos, const uvec3& dim){
        return pos.x+(pos.y*dim.x)+(pos.z*dim.x *dim.y);
    }

protected:
    void* data_;
    mutable NormalizedHistogram* histogram_;
};


/**
 * Factory for volumes.
 * Creates an VolumeRAM with data type specified by format. 
 * 
 * @param dimension of volume to create.
 * @param format of volume to create.
 * @return NULL if no valid format was specified.
 */
IVW_CORE_API VolumeRAM* createVolumeRAM(const uvec3& dimension, const DataFormatBase* format);

} // namespace

#endif // IVW_VOLUMERAM_H
