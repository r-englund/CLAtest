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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_DIFFUSE_LIGHT_SOURCE_PROCESSOR_H
#define IVW_DIFFUSE_LIGHT_SOURCE_PROCESSOR_H

#include <modules/base/basemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/light/diffuselight.h>
#include <inviwo/core/ports/dataoutport.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <inviwo/core/properties/vectorproperties.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

class DiffuseLight;

class IVW_MODULE_BASE_API DiffuseLightSourceProcessor : public Processor {
public:
    DiffuseLightSourceProcessor();
    virtual ~DiffuseLightSourceProcessor();

    InviwoProcessorInfo();

protected:
    virtual void process();

    /**
     * Update light source parameters. Transformation will be given in texture space.
     * 
     * @param lightSource 
     * @return 
     */
    void updateLightSource(DiffuseLight* lightSource);

private:
    DataOutport<LightSource> outport_;

    FloatProperty lightPowerProp_;
    FloatVec2Property lightSize_;

    FloatVec4Property lightDiffuse_;
    FloatVec3Property lightPosition_;

    DiffuseLight* lightSource_;
};

} // namespace

#endif // IVW_DIFFUSE_LIGHT_SOURCE_PROCESSOR_H
