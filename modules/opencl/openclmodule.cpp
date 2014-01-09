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
#include <modules/opencl/image/imageclconverter.h>
#include <modules/opencl/image/imageclglconverter.h>
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

OpenCLModule::OpenCLModule() : InviwoModule() {
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
    

    // ImageCL
    registerRepresentationConverter(new ImageDisk2CLConverter());
    registerRepresentationConverter(new ImageRAM2CLConverter());
    registerRepresentationConverter(new ImageCL2RAMConverter());
    registerRepresentationConverter(new ImageGL2CLConverter());
    registerRepresentationConverter(new ImageCL2GLConverter());

    // ImageCLGL
    registerRepresentationConverter(new ImageDisk2CLGLConverter());
    registerRepresentationConverter(new ImageRAM2CLGLConverter());
    registerRepresentationConverter(new ImageGL2CLGLConverter());
    registerRepresentationConverter(new ImageCL2CLGLConverter());
    registerRepresentationConverter(new ImageCLGL2RAMConverter());
    registerRepresentationConverter(new ImageCLGL2CLConverter());
    registerRepresentationConverter(new ImageCLGL2GLConverter());
    registerRepresentationConverter(new ImageGL2CLGLConverter());

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
            ButtonProperty* btnOpenCLInfo = new ButtonProperty("printOpenCLInfo", "Print OpenCL Info");
            btnOpenCLInfo->onChange(openclInfo, &OpenCLCapabilities::printInfo);
            settings->addProperty(btnOpenCLInfo);
        }           
    }
}

} // namespace
