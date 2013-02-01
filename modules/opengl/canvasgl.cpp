#include "canvasgl.h"

namespace inviwo {

bool CanvasGL::glewInitialized_ = false;
const std::string CanvasGL::logSource_ = "CanvasGL";

CanvasGL::CanvasGL(ivec2 dimensions)
    : Canvas(dimensions)
{}

void CanvasGL::initialize() {
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_COLOR_MATERIAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void CanvasGL::initializeGL() {
    if (!glewInitialized_) {
        LogInfo("Initializing GLEW");
        glewInit();
        glewInitialized_ = true;
    }
}

void CanvasGL::deinitialize() {}

void CanvasGL::switchContext() {}

void CanvasGL::repaint() {}

void CanvasGL::resize(ivec2 size) {
    Canvas::resize(size);
    glViewport(0, 0, size[0], size[1]);
}
void CanvasGL::update() {}

} // namespace
