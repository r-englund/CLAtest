#include <inviwo/core/common/inviwoapplication.h>
#include <modules/opengl/canvasprocessorgl.h>
#include <modules/opengl/geometry/geometryrenderprocessorgl.h>
#include <modules/opengl/geometry/meshgl.h>
#include <modules/opengl/glwrap/shadermanager.h>
#include <modules/opengl/image/imageglconverter.h>
#include <modules/opengl/openglmodule.h>
#include <modules/opengl/openglinfo.h>
#include <modules/opengl/volume/volumeglconverter.h>

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

    addRepresentationConverter(new MeshRAM2GLConverter());

    registerProcessor(CanvasProcessorGL);
    registerProcessor(GeometryRenderProcessorGL);

    addResourceInfo(new OpenGLInfo());
}

} // namespace
