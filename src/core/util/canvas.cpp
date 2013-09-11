#include <inviwo/core/util/canvas.h>

namespace inviwo {

EventHandler* eventHandler_();

Geometry* Canvas::screenAlignedSquare_ = NULL;

Canvas::Canvas(uvec2 dimensions)
               : dimensions_(dimensions),
                 processorNetworkEvaluator_(0)
{
    shared = true;
    if(!screenAlignedSquare_){
        shared = false;
        Position2dAttributes* vertices_ = new Position2dAttributes();
        vertices_->add(glm::vec2(-1.0f, -1.0f));
        vertices_->add(glm::vec2(1.0f, -1.0f));
        vertices_->add(glm::vec2(-1.0f, 1.0f));
        vertices_->add(glm::vec2(1.0f, 1.0f));
        TexCoord2dAttributes* texCoords_ = new TexCoord2dAttributes();
        texCoords_->add(glm::vec2(0.0f, 0.0f));
        texCoords_->add(glm::vec2(1.0f, 0.0f));
        texCoords_->add(glm::vec2(0.0f, 1.0f));
        texCoords_->add(glm::vec2(1.0f, 1.0f));
        MeshRAM* screenAlignedSquareMesh = new MeshRAM(GeometryRepresentation::TRIANGLES, GeometryRepresentation::STRIP);
        screenAlignedSquareMesh->addAttribute(vertices_);
        screenAlignedSquareMesh->addAttribute(texCoords_);
        screenAlignedSquare_ = new Geometry(screenAlignedSquareMesh);
    }
}

Canvas::~Canvas() {
    if(!shared)
        delete screenAlignedSquare_;
}

void Canvas::initialize() {}

void Canvas::deinitialize() {}

void Canvas::render(const Image* im){}

void Canvas::repaint() {}

void Canvas::activate() {}

void Canvas::resize(uvec2 size) {
    uvec2 previousDimensions = dimensions_;
    dimensions_ = size;
    if (processorNetworkEvaluator_) {
        ResizeEvent* resizeEvent = new ResizeEvent(dimensions_);
        resizeEvent->setPreviousSize(previousDimensions);        
        processorNetworkEvaluator_->propagateResizeEvent(this, resizeEvent);
        processorNetworkEvaluator_->evaluate();
    }
}

void Canvas::update() {}

} // namespace
