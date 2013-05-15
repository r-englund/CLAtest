#include "transferfunction.h"
#include "stdio.h"
#include <string.h>

namespace inviwo {

    TransferFunction::TransferFunction()
        : ProcessorGL(),
        outport_(Port::OUTPORT, "outport"),
        trans_("trans", "transfer", TransferFunc())
    {
        addPort(outport_);
        addProperty(trans_);
    }

    TransferFunction::~TransferFunction() {}

    Processor* TransferFunction::create() const {
        return new TransferFunction();
    }

    void TransferFunction::initialize() {
        ProcessorGL::initialize();
        shader_ = new Shader("img_transferfunction.frag");
    }

    void TransferFunction::deinitialize() {
        delete shader_;
        Processor::deinitialize();
    }

    void TransferFunction::process() {
        float* values = trans_.get().getAlpha();
        float val = values[0];
        Image* outImage = outport_.getData();
        ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
        uvec2 csize = outImageGL->getDimension();

        activateTarget(outport_);
        //bindColorTexture(inport0_, GL_TEXTURE0);
        LogInfoS("", val);
        shader_->activate();
        shader_->setUniform("value_", val);
        shader_->setUniform("dimension_", vec2(1.f / csize[0], 1.f / csize[1]) );
        renderImagePlaneQuad();
        shader_->deactivate();

        deactivateCurrentTarget();
    }

    bool TransferFunction::getSize()
    {
        throw std::exception("The method or operation is not implemented.");
    }

} // namespace
