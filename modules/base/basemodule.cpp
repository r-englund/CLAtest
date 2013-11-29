#include <modules/base/basemodule.h>

#include <modules/base/processors/diffuselightsourceprocessor.h>
#include <modules/base/processors/directionallightsourceprocessor.h>
#include <modules/base/processors/imagesource.h>
#include <modules/base/processors/imagesourceseries.h>
#include <modules/base/processors/meshclipping.h>
#include <modules/base/processors/meshcreator.h>
#include <modules/base/processors/testprocessor.h>
#include <modules/base/processors/transferfunctiontestprocessor.h>
#include <modules/base/processors/volumesource.h>
#include <modules/base/processors/volumeexport.h>
#include <modules/base/processors/volumebasistransformer.h>
#include <modules/base/processors/volumeslice.h>
#include <modules/base/processors/volumesubset.h>

namespace inviwo {

BaseModule::BaseModule() : InviwoModule() {
    setIdentifier("Base");
    setXMLFileName("base/basemodule.xml");

    registerProcessor(DiffuseLightSourceProcessor);
    registerProcessor(DirectionalLightSourceProcessor);
    registerProcessor(ImageSource);
    registerProcessor(ImageSourceSeries);
    registerProcessor(MeshClipping);
    registerProcessor(MeshCreator);
    registerProcessor(TestProcessor);
    registerProcessor(TransferFunctiontionTestProcessor);
	registerProcessor(VolumeSource);
	registerProcessor(VolumeExport);
    registerProcessor(VolumeBasisTransformer);
    registerProcessor(VolumeSlice);
    registerProcessor(VolumeSubset);
}

} // namespace
             