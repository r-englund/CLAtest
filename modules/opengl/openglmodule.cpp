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

void OpenGLModule::initialize() throw (Exception) {
//    glewInit();
}

void OpenGLModule::deinitialize() throw (Exception) {}

} // namespace
