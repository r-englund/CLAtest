#include "simpleraycaster.h"

namespace inviwo {

    SimpleRaycaster::SimpleRaycaster()
        : ProcessorGL(),
        volumePort_(Port::INPORT, "volume"),
        entryPort_(Port::INPORT, "entry-points"),
        exitPort_(Port::INPORT, "exit-points"),
        outport_(Port::OUTPORT, "outport")
    {
        addPort(volumePort_);
        addPort(entryPort_);
        addPort(exitPort_);
        addPort(outport_);
    }

    SimpleRaycaster::~SimpleRaycaster() {}

    void SimpleRaycaster::initialize() {
        ProcessorGL::initialize();
        shader_ = new Shader("raycasting.frag");
    }

    void SimpleRaycaster::deinitialize() {
        delete shader_;
        ProcessorGL::deinitialize();
    }

    void SimpleRaycaster::process() {
        activateTarget(outport_);
                
        bindColorTexture(entryPort_, GL_TEXTURE0);
        bindColorTexture(exitPort_, GL_TEXTURE1);

        Volume* volume = volumePort_.getData();
        VolumeGL* volumeGL = volume->getRepresentation<VolumeGL>();
        volumeGL->bindTexture(GL_TEXTURE2);

        shader_->activate();
        shader_->setUniform("entryTex_", 0);
        shader_->setUniform("exitTex_", 1);
        shader_->setUniform("volume_", 2);
        renderImagePlaneQuad();
        shader_->deactivate();

        deactivateCurrentTarget();
    }

} // namespace
