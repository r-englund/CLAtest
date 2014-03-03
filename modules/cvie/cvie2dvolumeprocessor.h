/**********************************************************************
 * Copyright (C) 2104 ContextVision AB
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

#ifndef IVW_CVIE2DVOLUMEPROCESSOR_H
#define IVW_CVIE2DVOLUMEPROCESSOR_H

#include <modules/cvie/cviemoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/directoryproperty.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <CVIE.h>

namespace inviwo {

class IVW_MODULE_CVIE_API CVIE2DVolumeProcessor : public Processor {
public:
    CVIE2DVolumeProcessor();
    ~CVIE2DVolumeProcessor();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();
    void passthrough();

    bool createCVIEInstance();
    void destroyCVIEInstance();

    bool setupEnhancement();
    bool runEnhancement();

    void updateParameterFile();

    void findParameterFiles();
    void sliceOnlyChanged();

private:
    VolumeInport inport_;
    VolumeOutport outport_;

    BoolProperty enabled_;
    BoolProperty onlyProcessSelectedSlice_;
    IntProperty sliceNumber_;
    DirectoryProperty parameterFileDirectory_;
    ButtonProperty findParameterFiles_;
    OptionPropertyString parameterFile_;
    IntProperty parameterSetting_;

    HCVIE cvieHandle_;
    bool cvieContextCreated_;
};

} // namespace

#endif // IVW_CVIE2DVOLUMEPROCESSOR_H
