#include <modules/opengl/openglmodule.h>

#include <modules/opengl/canvasprocessorgl.h>
#include <modules/opengl/geometry/trianglemeshgl.h>
#include <modules/opengl/image/imageglconverter.h>
#include <modules/opengl/openglinfo.h>
#include <modules/opengl/volume/volumeglconverter.h>
#include <modules/opengl/glwrap/shadermanager.h>

#include <inviwo/core/common/inviwoapplication.h>


namespace inviwo {

OpenGLModule::OpenGLModule() : InviwoModule() {
    setIdentifier("OpenGL");
    setXMLFileName("opengl/openglmodule.xml");

    ShaderManager::init();
    ShaderManager::getPtr()->addShaderSearchPath(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_MODULES)+"opengl/glsl");

    addRepresentationConverter(new ImageRAM2GLConverter());
    addRepresentationConverter(new ImageGL2RAMConverter());
    addRepresentationConverter(new ImageDisk2GLConverter());

    addRepresentationConverter(new VolumeRAM2GLConverter());
    addRepresentationConverter(new VolumeDisk2GLConverter());

    addRepresentationConverter(new TriangleMeshRAM2GLConverter());

    registerProcessor(CanvasProcessorGL);

    addResourceInfo(new OpenGLInfo());
}

} // namespace
