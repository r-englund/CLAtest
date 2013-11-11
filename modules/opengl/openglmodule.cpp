#include <inviwo/core/common/inviwoapplication.h>
#include <modules/opengl/buffer/bufferglconverter.h>
#include <modules/opengl/buffer/elementbuffergl.h>
#include <modules/opengl/canvasprocessorgl.h>
#include <modules/opengl/geometry/geometryrenderprocessorgl.h>
#include <modules/opengl/glwrap/shadermanager.h>
#include <modules/opengl/image/imageglconverter.h>
#include <modules/opengl/openglmodule.h>
#include <modules/opengl/openglcapabilities.h>
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

    addRepresentationConverter(new BufferRAM2GLConverter());
    addRepresentationConverter(new BufferRAM2ElementGLConverter());

    registerProcessor(CanvasProcessorGL);
    registerProcessor(GeometryRenderProcessorGL);

    addCapabilities(new OpenGLCapabilities());
}

void OpenGLModule::setupModuleSettings(){
    if (getSettings()){
        OpenGLCapabilities* openglInfo = getTypeFromVector<OpenGLCapabilities>(getCapabilities());
        if (openglInfo){
            ButtonProperty* btnOpenGLInfo = new ButtonProperty("printOpenGLInfo", "Print OpenGL Info");
            btnOpenGLInfo->registerClassMemberFunction(openglInfo, &OpenGLCapabilities::printInfo);
            getSettings()->addProperty(btnOpenGLInfo);
        }           
    }
}

} // namespace
