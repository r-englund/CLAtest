/**********************************************************************
 * Copyright (C) 2014 ContextVision AB
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

#ifndef IVW_CVSCPROCESSOR_H
#define IVW_CVSCPROCESSOR_H

#include <modules/cvsc/cvscmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/volumeport.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/boolproperty.h>
#include <inviwo/core/properties/fileproperty.h>
#include <cvsc.h>

namespace inviwo {

class IVW_MODULE_CVSC_API CVSCProcessor : public Processor {
public:
    CVSCProcessor();
    ~CVSCProcessor();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();
    void passthrough();

    bool initCVSCInstance();
    void destroyCVSCInstance();

    bool setupScanConversion();
    bool runScanConversion();

    void updateParameterFile();

private:
    VolumeInport inport_;
    VolumeOutport outport_;

    BoolProperty enabled_;
    FileProperty parameterFile_;

    CVSC_Parameters params_;
    CVSC_Volume outVolume_;
    bool cvscInit_;
};

} // namespace

#endif // IVW_CVSCPROCESSOR_H
