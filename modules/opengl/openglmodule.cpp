/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
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

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/util/vectoroperations.h>
#include <inviwo/core/util/settings/systemsettings.h>
#include <modules/opengl/buffer/bufferglconverter.h>
#include <modules/opengl/buffer/elementbuffergl.h>
#include <modules/opengl/canvasprocessorgl.h>
#include <modules/opengl/geometry/geometryrenderprocessorgl.h>
#include <modules/opengl/glwrap/shadermanager.h>
#include <modules/opengl/image/layerglconverter.h>
#include <modules/opengl/openglmodule.h>
#include <modules/opengl/openglcapabilities.h>
#include <modules/opengl/volume/volumeglconverter.h>

namespace inviwo {

OpenGLModule::OpenGLModule() : InviwoModule() {
    setIdentifier("OpenGL");
    setXMLFileName("opengl/openglmodule.xml");

    ShaderManager::init();
    ShaderManager::getPtr()->addShaderSearchPath(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+"opengl/glsl");

    registerRepresentationConverter(new LayerRAM2GLConverter());
    registerRepresentationConverter(new LayerGL2RAMConverter());
    registerRepresentationConverter(new LayerDisk2GLConverter());

    registerRepresentationConverter(new VolumeRAM2GLConverter());
    registerRepresentationConverter(new VolumeDisk2GLConverter());

    registerRepresentationConverter(new BufferRAM2GLConverter());
    registerRepresentationConverter(new BufferRAM2ElementGLConverter());

    registerProcessor(CanvasProcessorGL);
    registerProcessor(GeometryRenderProcessorGL);

    registerCapabilities(new OpenGLCapabilities());
}

void OpenGLModule::setupModuleSettings(){
    //New OpengGLSettings class can be created if required.
    SystemSettings* settings = InviwoApplication::getPtr()->getSettingsByType<SystemSettings>();
    if (settings){
        OpenGLCapabilities* openglInfo = getTypeFromVector<OpenGLCapabilities>(getCapabilities());
        if (openglInfo){
            ButtonProperty* btnOpenGLInfo = new ButtonProperty("printOpenGLInfo", "Print OpenGL Info");
            btnOpenGLInfo->onChange(openglInfo, &OpenGLCapabilities::printInfo);
            settings->addProperty(btnOpenGLInfo);
        }           
    }
}

} // namespace
