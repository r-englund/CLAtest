#include "canvasprocessorgl.h"

namespace inviwo {

CanvasProcessorGL::CanvasProcessorGL()
    : CanvasProcessor(),
      dimensions_("dimensions", "Dimensions", ivec2(256,256), ivec2(128,128), ivec2(4096,4096))
{
    dimensions_.onChange(this, &CanvasProcessorGL::resizeCanvas);
    addProperty(dimensions_);
}

Processor* CanvasProcessorGL::create() const {
    return new CanvasProcessorGL();
}

void CanvasProcessorGL::resizeCanvas() {
    //std::cout << "onChange" << std::endl;
    //getCanvas()->resize(uvec2(dimensions_.get()));
}

void CanvasProcessorGL::initialize() {
    CanvasProcessor::initialize();
    shader_ = new Shader("img_texturequad.frag");
}

void CanvasProcessorGL::deinitialize() {
    delete shader_;
    CanvasProcessor::deinitialize();
}

void CanvasProcessorGL::renderImagePlaneQuad() const {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glDepthFunc(GL_ALWAYS);
    glBegin(GL_QUADS);
    glTexCoord2f(0.f, 0.f); glVertex2f(-1.f, -1.f);
    glTexCoord2f(1.f, 0.f); glVertex2f( 1.f, -1.f);
    glTexCoord2f(1.f, 1.f); glVertex2f( 1.f,  1.f);
    glTexCoord2f(0.f, 1.f); glVertex2f(-1.f,  1.f);
    glEnd();
    glDepthFunc(GL_LESS);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void CanvasProcessorGL::process() {
    CanvasProcessor::process();
    Image* inImage = inport_.getData();

    //Temporary line because of converterpackage not working (data.h)
    //this line forces a ImageRAM representation to be made which results
    //in a representation that we can convert to a ImageGL.
    ImageRAM* temp = inImage->getRepresentation<ImageRAM>();
    
    ImageGL* inImageGL = inImage->getRepresentation<ImageGL>();
    if(inImageGL == NULL)
        inImageGL = new ImageGL();
    uvec2 csize = getCanvas()->size();
    inImageGL->bindColorTexture(GL_TEXTURE0);

    shader_->activate();
    shader_->setUniform("colorTex_", 0);
    shader_->setUniform("dimension_", vec2( 1.f / csize[0],  1.f / csize[1]) );
    renderImagePlaneQuad();
    shader_->deactivate();
    inImageGL->unbindColorTexture();
}

} // namespace
