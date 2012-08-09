#include "imagesource.h"

namespace inviwo {

    ImageSource::ImageSource()
        : ProcessorGL(),
        outport_(Port::OUTPORT, "image.outport")
    {
        addPort(outport_);
    }

    void ImageSource::initialize() {
        Processor::initialize();
        shader_ = new Shader("img_texturequad.frag");
    }

    void ImageSource::deinitialize() {
        delete shader_;
        Processor::deinitialize();
    }

    void ImageSource::process() {
        std::cout << "ImageSource::process()" << std::endl;

        Image* outImage = outport_.getData();
        ImageGL* outImageGL = outImage->getRepresentation<ImageGL>();
        outImageGL->activateBuffer();

        Texture2D* testTex = new Texture2D(ivec2(512,512), GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, GL_LINEAR);
        glActiveTexture(GL_TEXTURE0);
        testTex->loadTexture("D:/inviwo/data/images/swirl.tga");

        shader_->activate();
        shader_->setUniform("colorTex_", 0);
        renderImagePlaneQuad();
        shader_->deactivate();

        outImageGL->deactivateBuffer();
    }

} // namespace
