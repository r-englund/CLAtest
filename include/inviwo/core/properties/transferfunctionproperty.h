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

namespace inviwo {

class IVW_CORE_API TransferFunctionProperty : public TemplateProperty<TransferFunction> {

public:
    TransferFunctionProperty(std::string identifier, std::string displayName, TransferFunction value = TransferFunction(),
                             PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                             PropertySemantics::Type semantics=PropertySemantics::Default);
    ~TransferFunctionProperty();

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "TransferFunctionProperty"; }

    vec2 getMask() { return mask_; }
    void setMask(float maskMin, float maskMax) { if (maskMax<maskMin) maskMax=maskMin; mask_ = vec2(maskMin, maskMax); get().setMaskMin(mask_.x); get().setMaskMax(mask_.y); }

    vec2 getZoomH() { return zoomH_; }
    void setZoomH(float zoomHMin, float zoomHMax) { if (zoomHMax<zoomHMin) zoomHMax=zoomHMin; zoomH_ = vec2(zoomHMin, zoomHMax); }

    vec2 getZoomV() { return zoomV_; }
    void setZoomV(float zoomVMin, float zoomVMax) { if (zoomVMax<zoomVMin) zoomVMax=zoomVMin; zoomV_ = vec2(zoomVMin, zoomVMax); }

    void setVolume(const Volume* volume);
    std::vector<float> getHistogram() { return histogram_; }

private:
    vec2 mask_;
    vec2 zoomH_;
    vec2 zoomV_;

    const Volume* volume_;
    std::vector<float> histogram_;
};

} // namespace inviwo

#endif // IVW_TRANSFERFUNCTIONPROPERTY_H