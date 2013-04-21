#include "snapshotprocessor.h"
#include <cmath>

namespace inviwo {

SnapshotProcessor::SnapshotProcessor()
        : ProcessorGL(),      
	    inport0_(Port::INPORT, "inport0"),
	    outport_(Port::OUTPORT, "outport"),
        imageFileName_("imageFileName", "Image file name", IVW_DIR+"data/images/output.png"),
        buttonPropperty_("takeSnapShot", "Take snapshot")
        //snapshotProperty_("snapshot", "Take snapshot", inport0_)
{
    addPort(inport0_);
    addPort(outport_);
    addProperty(imageFileName_);
    addProperty(buttonPropperty_);
    //addProperty(snapshotProperty_);
    buttonPropperty_.registerClassMemberFunction(this, &SnapshotProcessor::buttonAction);
}

SnapshotProcessor::~SnapshotProcessor() {}

Processor* SnapshotProcessor::create() const {
    return new SnapshotProcessor();
}

void SnapshotProcessor::initialize() {
    Processor::initialize();
    shader_ = new Shader("img_texturequad.frag");

}

void SnapshotProcessor::deinitialize() {
    delete shader_;
    Processor::deinitialize();
}


void SnapshotProcessor::buttonAction(){
    const Image* inputImage = inport0_.getData();

    ImageLoader::saveImage(imageFileName_.get().c_str(), inputImage);
}


void SnapshotProcessor::process() {
    const Image* inputImage = inport0_.getData();
    Image* outImage = outport_.getEditableData();
    
    const ImageGL* inImageGL = inputImage->getRepresentation<ImageGL>();
    ImageGL* outImageGL = outImage->getEditableRepresentation<ImageGL>();

    uvec2 imageSize = inImageGL->getDimension();
    outImageGL->resize(imageSize);

    activateTarget(outport_);
    bindColorTexture(inport0_, GL_TEXTURE0);

    shader_->activate();
    shader_->setUniform("inport0_", 0);
    shader_->setUniform("dimension_", vec2(1.f / imageSize[0], 1.f / imageSize[1]) );
    renderImagePlaneQuad();
    shader_->deactivate();

    deactivateCurrentTarget();
}

} // namespace
