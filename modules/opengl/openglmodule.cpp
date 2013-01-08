#include "modules/opengl/openglmodule.h"

#include "modules/opengl/canvasprocessorgl.h"
#include "modules/opengl/imageglconverter.h"
#include "modules/opengl/volumeglconverter.h"

namespace inviwo {

OpenGLModule::OpenGLModule() : InviwoModule() {
    setIdentifier("OpenGL");
    setXMLFileName("opengl/openglmodule.xml");

    addRepresentationConverter(new ImageRAM2GLConverter());

    addRepresentationConverter(new VolumeRAM2GLConverter());
    addRepresentationConverter(new VolumeDisk2GLConverter());

    addProcessor(new CanvasProcessorGL());
}

void OpenGLModule::initialize() {
//    glewInit();
}

void OpenGLModule::deinitialize() {}

} // namespace
