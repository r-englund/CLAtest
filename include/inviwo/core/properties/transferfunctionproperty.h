/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Viktor Axelsson
 *
 **********************************************************************/

#ifndef IVW_TRANSFERFUNCTIONPROPERTY_H
#define IVW_TRANSFERFUNCTIONPROPERTY_H

#include <inviwo/core/properties/templateproperty.h>
#include <inviwo/core/properties/vectorproperties.h>
#include <inviwo/core/datastructures/transferfunction.h>
#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/datastructures/histogram.h>
#include <inviwo/core/ports/volumeport.h>

namespace inviwo {

class IVW_CORE_API TransferFunctionProperty : public TemplateProperty<TransferFunction> {

public:
    TransferFunctionProperty(std::string identifier, std::string displayName, TransferFunction value = TransferFunction(),
                             VolumeInport* volumeInport = NULL, PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                             PropertySemantics semantics=PropertySemantics::Default);
    ~TransferFunctionProperty();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "TransferFunctionProperty"; }

    const vec2& getMask() const { return mask_; }
    void setMask(float maskMin, float maskMax) { if (maskMax<maskMin) maskMax=maskMin; mask_ = vec2(maskMin, maskMax); get().setMaskMin(mask_.x); get().setMaskMax(mask_.y); }

    const vec2& getZoomH() const { return zoomH_; }
    void setZoomH(float zoomHMin, float zoomHMax) { if (zoomHMax<zoomHMin) zoomHMax=zoomHMin; zoomH_ = vec2(zoomHMin, zoomHMax); }

    const vec2& getZoomV() const { return zoomV_; }
    void setZoomV(float zoomVMin, float zoomVMax) { if (zoomVMax<zoomVMin) zoomVMax=zoomVMin; zoomV_ = vec2(zoomVMin, zoomVMax); }
    
    VolumeInport* getVolumeInport();

private:
    vec2 mask_;
    vec2 zoomH_;
    vec2 zoomV_;

    VolumeInport* volumeInport_;
};

} // namespace inviwo

#endif // IVW_TRANSFERFUNCTIONPROPERTY_H