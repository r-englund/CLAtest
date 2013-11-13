#include <modules/base/basemodule.h>

#include <modules/base/processors/background.h>
#include <modules/base/processors/cubeproxygeometry.h>
#include <modules/base/processors/drawlinesprocessor.h>
#include <modules/base/processors/entryexitpoints.h>
#include <modules/base/processors/findedges.h>
#include <modules/base/processors/firstivwprocessor.h>
#include <modules/base/processors/imagegrayscale.h>
#include <modules/base/processors/imagemixer.h>
#include <modules/base/processors/imagesource.h>
#include <modules/base/processors/imagesourceseries.h>
#include <modules/base/processors/meshclipping.h>
#include <modules/base/processors/meshcreator.h>
#include <modules/base/processors/parallelepipedproxygeometry.h>
#include <modules/base/processors/positionwidgetprocessor.h>
#include <modules/base/processors/redgreenprocessor.h>
#include <modules/base/processors/simpleraycaster.h>
#include <modules/base/processors/testprocessor.h>
#include <modules/base/processors/transferfunctiontestprocessor.h>
#include <modules/base/processors/volumebasistransformer.h>
#include <modules/base/processors/volumeslice.h>
#include <modules/base/processors/volumesplit.h>
#include <modules/base/processors/volumesplitcompositor.h>
#include <modules/base/processors/volumesplitimagecompositor.h>
#include <modules/base/processors/volumesubset.h>

namespace inviwo {

BaseModule::BaseModule() : InviwoModule() {
    setIdentifier("Base");
    setXMLFileName("base/basemodule.xml");

	registerProcessor(Background);
	registerProcessor(CubeProxyGeometry);
    registerProcessor(DrawLinesProcessor);
    registerProcessor(EntryExitPoints);
	registerProcessor(FindEdges);
	registerProcessor(FirstIvwProcessor);
	registerProcessor(ImageGrayscale);
    registerProcessor(ImageMixer);
    registerProcessor(ImageSource);
    registerProcessor(ImageSourceSeries);
    registerProcessor(MeshClipping);
    registerProcessor(MeshCreator);
    registerProcessor(ParallelepipedProxyGeometry);
    registerProcessor(PositionWidgetProcessor);
    registerProcessor(RedGreenProcessor);
    registerProcessor(SimpleRaycaster);
    registerProcessor(TestProcessor);
    registerProcessor(TransferFunctiontionTestProcessor);
    registerProcessor(VolumeBasisTransformer);
    registerProcessor(VolumeSlice);
    registerProcessor(VolumeSplit);
    registerProcessor(VolumeSplitCompositor);
    registerProcessor(VolumeSplitImageCompositor);
    registerProcessor(VolumeSubset);
}

} // namespace
             