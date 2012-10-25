#include "canvasgl.h"

namespace inviwo {

CanvasGL::CanvasGL(ivec2 dimensions)
    : Canvas(dimensions)
{}

void CanvasGL::initialize() {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void CanvasGL::deinitialize() {}

void CanvasGL::switchContext() {}

void CanvasGL::repaint() {}

void CanvasGL::resize(ivec2 size) {
    Canvas::resize(size);
}
void CanvasGL::update() {}

} // namespace
