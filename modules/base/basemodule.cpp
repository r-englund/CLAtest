#include <modules/base/basemodule.h>

#include <modules/base/processors/cubeproxygeometry.h>
#include <modules/base/processors/entryexitpoints.h>
#include <modules/base/processors/findedges.h>
#include <modules/base/processors/firstivwprocessor.h>
#include <modules/base/processors/imagegrayscale.h>
#include <modules/base/processors/imagemixer.h>
#include <modules/base/processors/imagesource.h>
#include <modules/base/processors/meshclipping.h>
#include <modules/base/processors/redgreenprocessor.h>
#include <modules/base/processors/simpleraycaster.h>
#include <modules/base/processors/testprocessor.h>
#include <modules/base/processors/transferfunctiontestprocessor.h>
#include <modules/base/processors/volumesplit.h>
#include <modules/base/processors/volumesplitcompositor.h>
#include <modules/base/processors/volumesplitimagecompositor.h>

namespace inviwo {

BaseModule::BaseModule() : InviwoModule() {
    setIdentifier("Base");
    setXMLFileName("base/basemodule.xml");

    registerProcessor(CubeProxyGeometry);
    registerProcessor(EntryExitPoints);
	registerProcessor(FindEdges);
	registerProcessor(FirstIvwProcessor);
	registerProcessor(ImageGrayscale);
    registerProcessor(ImageMixer);
    registerProcessor(ImageSource);
    registerProcessor(MeshClipping);
    registerProcessor(RedGreenProcessor);
    registerProcessor(SimpleRaycaster);
    registerProcessor(TestProcessor);
    registerProcessor(TransferFunctiontionTestProcessor);
    registerProcessor(VolumeSplit);
    registerProcessor(VolumeSplitCompositor);
    registerProcessor(VolumeSplitImageCompositor);
}

} // namespace
