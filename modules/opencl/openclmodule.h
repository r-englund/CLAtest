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

#ifndef IVW_OPENCLMODULE_H
#define IVW_OPENCLMODULE_H

#include <inviwo/core/common/inviwomodule.h>
#include <modules/opencl/openclmoduledefine.h>
namespace inviwo {
/*
 * See inviwoopencl.h for usage.
 */ 
class IVW_MODULE_OPENCL_API OpenCLModule : public InviwoModule {

public:
    OpenCLModule();

    void initialize();
    void deinitialize();

protected:
    void setupModuleSettings();

};

} // namespace

#endif // IVW_OPENGLMODULE_H
