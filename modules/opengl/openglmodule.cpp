#include <modules/opengl/openglmodule.h>

#include <modules/opengl/glwrap/shadermanager.h>

#include <modules/opengl/canvasprocessorgl.h>
#include <modules/opengl/imageglconverter.h>
#include <modules/opengl/volumeglconverter.h>
#include <inviwo/core/datastructures/imageramconverter.h>

#include <modules/opengl/openglinfo.h>

namespace inviwo {

OpenGLModule::OpenGLModule() : InviwoModule() {
    setIdentifier("OpenGL");
    setXMLFileName("opengl/openglmodule.xml");

    ShaderManager::init();
    addRepresentationConverter(new ImageDisk2RAMConverter());
    addRepresentationConverter(new ImageRAM2GLConverter());
    addRepresentationConverter(new ImageGL2RAMConverter());
    addRepresentationConverter(new VolumeRAM2GLConverter());
    addRepresentationConverter(new VolumeDisk2GLConverter());
    

    addProcessor(new CanvasProcessorGL());

    addResourceInfo(new OpenGLInfo());
}

} // namespace
