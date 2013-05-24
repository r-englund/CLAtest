#include <modules/opengl/openglmodule.h>

#include <modules/opengl/glwrap/shadermanager.h>
#include <modules/opengl/canvasprocessorgl.h>
#include <modules/opengl/imageglconverter.h>
#include <modules/opengl/volumeglconverter.h>

#include <modules/opengl/openglinfo.h>

namespace inviwo {

OpenGLModule::OpenGLModule() : InviwoModule() {
    setIdentifier("OpenGL");
    setXMLFileName("opengl/openglmodule.xml");

    ShaderManager::init();

    addRepresentationConverter(new ImageRAM2GLConverter());
    addRepresentationConverter(new ImageGL2RAMConverter());
    addRepresentationConverter(new ImageDisk2GLConverter());

    addRepresentationConverter(new VolumeRAM2GLConverter());
    addRepresentationConverter(new VolumeDisk2GLConverter());

    registerProcessor(CanvasProcessorGL);

    addResourceInfo(new OpenGLInfo());
}

} // namespace
