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

#include <modules/opencl/inviwoopencl.h>
#include <modules/opencl/buffer/bufferclconverter.h>
#include <modules/opencl/buffer/bufferclglconverter.h>
#include <modules/opencl/image/layerclconverter.h>
#include <modules/opencl/image/layerclglconverter.h>
#include <modules/opencl/openclmodule.h>
#include <modules/opencl/openclcapabilities.h>
#include <modules/opencl/processors/grayscalecl.h>
#include <modules/opencl/processors/volumeraycastercl.h>
#include <modules/opencl/volume/volumeclconverter.h>
#include <modules/opencl/volume/volumeclglconverter.h>
#include <modules/opencl/kernelmanager.h>

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/vectoroperations.h>
#include <inviwo/core/util/settings/systemsettings.h>

namespace inviwo {

OpenCLModule::OpenCLModule() : InviwoModule(), btnOpenCLInfo_("printOpenCLInfo", "Print OpenCL Info") {
    setIdentifier("OpenCL");
    setXMLFileName("opencl/openclmodule.xml");
}

void OpenCLModule::initialize() {
    
    OpenCL::instance()->addCommonIncludeDirectory(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+"opencl/cl");
    KernelManager::init();
    // Buffer CL
    registerRepresentationConverter(new BufferRAM2CLConverter());
    registerRepresentationConverter(new BufferCL2RAMConverter());
    registerRepresentationConverter(new BufferGL2CLConverter());

    // Buffer CLGL
    registerRepresentationConverter(new BufferRAM2CLGLConverter());
    registerRepresentationConverter(new BufferCLGL2RAMConverter());
    registerRepresentationConverter(new BufferCL2GLConverter());
    registerRepresentationConverter(new BufferCLGL2CLConverter());
    registerRepresentationConverter(new BufferCL2CLGLConverter());
    

    // LayerCL
    registerRepresentationConverter(new LayerDisk2CLConverter());
    registerRepresentationConverter(new LayerRAM2CLConverter());
    registerRepresentationConverter(new LayerCL2RAMConverter());
    registerRepresentationConverter(new LayerGL2CLConverter());
    registerRepresentationConverter(new LayerCL2GLConverter());

    // LayerCLGL
    registerRepresentationConverter(new LayerDisk2CLGLConverter());
    registerRepresentationConverter(new LayerRAM2CLGLConverter());
    registerRepresentationConverter(new LayerGL2CLGLConverter());
    registerRepresentationConverter(new LayerCL2CLGLConverter());
    registerRepresentationConverter(new LayerCLGL2RAMConverter());
    registerRepresentationConverter(new LayerCLGL2CLConverter());
    registerRepresentationConverter(new LayerCLGL2GLConverter());
    registerRepresentationConverter(new LayerGL2CLGLConverter());

    // VolumeCL
    registerRepresentationConverter(new VolumeDisk2CLConverter());
    registerRepresentationConverter(new VolumeRAM2CLConverter());
    registerRepresentationConverter(new VolumeCL2RAMConverter());

    // VolumeCLGL
    registerRepresentationConverter(new VolumeDisk2CLGLConverter());
    registerRepresentationConverter(new VolumeRAM2CLGLConverter());
    registerRepresentationConverter(new VolumeGL2CLGLConverter());
    registerRepresentationConverter(new VolumeCL2CLGLConverter());
    registerRepresentationConverter(new VolumeCLGL2RAMConverter());
    registerRepresentationConverter(new VolumeCLGL2CLConverter());
    registerRepresentationConverter(new VolumeGL2CLGLConverter());


    registerCapabilities(new OpenCLCapabilities());
    // Processors
    registerProcessor(GrayscaleCL);
    registerProcessor(VolumeRaycasterCL);
    InviwoModule::initialize();
}

void OpenCLModule::deinitialize() {
    
}

void OpenCLModule::setupModuleSettings(){
    //New OpengCLSettings class can be created if required.
    SystemSettings* settings = InviwoApplication::getPtr()->getSettingsByType<SystemSettings>();
    if (settings){
        OpenCLCapabilities* openclInfo = getTypeFromVector<OpenCLCapabilities>(getCapabilities());
        if (openclInfo){
            btnOpenCLInfo_.onChange(openclInfo, &OpenCLCapabilities::printInfo);
            settings->addProperty(&btnOpenCLInfo_);
        }           
    }
}

} // namespace
