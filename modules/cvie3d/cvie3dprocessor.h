/**********************************************************************
 * Copyright (C) 2013 Vistinct AB
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

#ifndef IVW_CVIE3DPROCESSOR_H
#define IVW_CVIE3DPROCESSOR_H

#include <modules/cvie3d/cvie3dmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/util/filedirectory.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <CVIE3D.h>

namespace inviwo {

class IVW_MODULE_CVIE3D_API CVIE3DProcessor : public Processor {
public:
    CVIE3DProcessor();
    ~CVIE3DProcessor();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();
    void passthrough();

    bool createCVIE3DInstance();
    void destroyCVIE3DInstance();

    bool setupEnhancement();
    bool runEnhancement();

    void updateConfigurationFile();
    void updateParameterFile();

private:
    VolumeInport inport_;
    VolumeOutport outport_;

    BoolProperty enabled_;
    FileProperty confFile_;
    FileProperty parameterFile_;
    IntProperty parameterSetting_;

    HCVIE3D cvieHandle_;
    bool cvieContextCreated_;
};

} // namespace

#endif // IVW_CVIE3DPROCESSOR_H
