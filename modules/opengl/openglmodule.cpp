#include "modules/opengl/openglmodule.h"

#include "modules/opengl/canvasprocessorgl.h"
#include "modules/opengl/imageglconverter.h"

namespace inviwo {

OpenGLModule::OpenGLModule() : InviwoModule() {
    setIdentifier("OpenGL");
    setXMLFileName("opengl/openglmodule.xml");

    addRepresentationConverter(new ImageGLConverter());

    addProcessor(new CanvasProcessorGL());
}

void OpenGLModule::initialize() {
//    glewInit();
}

void OpenGLModule::deinitialize() {}

} // namespace
