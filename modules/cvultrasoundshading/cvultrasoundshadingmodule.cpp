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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#include <modules/cvultrasoundshading/cvultrasoundshadingmodule.h>

#include <modules/cvultrasoundshading/processors/volumesepiashadingcl.h>

namespace inviwo {

CVUltrasoundShadingModule::CVUltrasoundShadingModule() : InviwoModule() {
    setIdentifier("CVUltrasoundShading");

    registerProcessor(VolumeSepiaShadingCL);
}

void CVUltrasoundShadingModule::initialize() {
    OpenCL::instance()->addCommonIncludeDirectory(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+"cvultrasoundshading/cl");
    InviwoModule::initialize();
}

CVUltrasoundShadingModule::~CVUltrasoundShadingModule()
{

}

} // namespace
             