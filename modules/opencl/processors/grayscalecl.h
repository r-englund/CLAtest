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

#ifndef IVW_GRAYSCALE_CL_H
#define IVW_GRAYSCALE_CL_H

#include <modules/opencl/openclmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/processors/processor.h>



#include <modules/opencl/inviwoopencl.h>

namespace inviwo {

class IVW_MODULE_OPENCL_API GrayscaleCL : public Processor {
public:
    GrayscaleCL();
    ~GrayscaleCL();
    
    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

protected:
    virtual void process();

private:
    ImageInport inputPort_;
    ImageOutport outport_;

    cl::Kernel* kernel_;
};

} // namespace

#endif // IVW_GRAYSCALE_CL_H
