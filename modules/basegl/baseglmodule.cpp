#include <modules/basegl/baseglmodule.h>

#include <modules/basegl/processors/background.h>
#include <modules/basegl/processors/cubeproxygeometry.h>
#include <modules/basegl/processors/drawlinesprocessor.h>
#include <modules/basegl/processors/entryexitpoints.h>
#include <modules/basegl/processors/findedges.h>
#include <modules/basegl/processors/firstivwprocessor.h>
#include <modules/basegl/processors/imageclassify.h>
#include <modules/basegl/processors/imagegrayscale.h>
#include <modules/basegl/processors/imagemixer.h>
#include <modules/basegl/processors/positionwidgetprocessor.h>
#include <modules/basegl/processors/redgreenprocessor.h>
#include <modules/basegl/processors/simpleraycaster.h>
#include <modules/basegl/processors/volumeslicegl.h>

namespace inviwo {

BaseGLModule::BaseGLModule() : InviwoModule() {
    setIdentifier("BaseGL");
    setXMLFileName("basegl/baseglmodule.xml");

	registerProcessor(Background);
	registerProcessor(CubeProxyGeometry);
    registerProcessor(DrawLinesProcessor);
    registerProcessor(EntryExitPoints);
	registerProcessor(FindEdges);
	registerProcessor(FirstIvwProcessor);
    registerProcessor(ImageClassify);
	registerProcessor(ImageGrayscale);
    registerProcessor(ImageMixer);
    registerProcessor(PositionWidgetProcessor);
    registerProcessor(RedGreenProcessor);
    registerProcessor(SimpleRaycaster);
    registerProcessor(VolumeSliceGL);
}

} // namespace
             