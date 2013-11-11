#include <inviwo/core/util/canvas.h>
#include <inviwo/core/datastructures/geometry/mesh.h>

namespace inviwo {

EventHandler* eventHandler_();

Geometry* Canvas::screenAlignedRect_ = NULL;

Canvas::Canvas(uvec2 dimensions)
               : dimensions_(dimensions),
                 processorNetworkEvaluator_(0)
{
    shared_ = true;
    pickingContainer_ = new PickingContainer();
    if(!screenAlignedRect_){
        shared_ = false;
        Position2dBuffer* verticesBuffer = new Position2dBuffer();
        Position2dBufferRAM* verticesBufferRAM = verticesBuffer->getEditableRepresentation<Position2dBufferRAM>();
        verticesBufferRAM->add(vec2(-1.0f, -1.0f));
        verticesBufferRAM->add(vec2(1.0f, -1.0f));
        verticesBufferRAM->add(vec2(-1.0f, 1.0f));
        verticesBufferRAM->add(vec2(1.0f, 1.0f));
        TexCoord2dBuffer* texCoordsBuffer = new TexCoord2dBuffer();
        TexCoord2dBufferRAM* texCoordsBufferRAM = texCoordsBuffer->getEditableRepresentation<TexCoord2dBufferRAM>();
        texCoordsBufferRAM->add(vec2(0.0f, 0.0f));
        texCoordsBufferRAM->add(vec2(1.0f, 0.0f));
        texCoordsBufferRAM->add(vec2(0.0f, 1.0f));
        texCoordsBufferRAM->add(vec2(1.0f, 1.0f));
        Mesh* screenAlignedRectMesh = new Mesh(TRIANGLES, STRIP);
        screenAlignedRectMesh->addAttribute(verticesBuffer);
        screenAlignedRectMesh->addAttribute(texCoordsBuffer);
        screenAlignedRect_ = screenAlignedRectMesh;
    }
}

Canvas::~Canvas() {
    if(!shared_)
        delete screenAlignedRect_;
    delete pickingContainer_;
}

void Canvas::initialize() {
    if(!pickingContainer_)
        pickingContainer_ = new PickingContainer();
}

void Canvas::deinitialize() {}

void Canvas::render(const Image* im) {}

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

void Canvas::interactionEvent(InteractionEvent* e) {
    processorNetworkEvaluator_->propagateInteractionEvent(this, e);
    processorNetworkEvaluator_->evaluate();
}

void Canvas::mousePressEvent(MouseEvent* e) {
    if(e->button() == MouseEvent::MOUSE_BUTTON_LEFT) {
        bool picked = pickingContainer_->performPick(mousePosToPixelCoordinates(e->pos()));
        if(!picked)
            interactionEvent(e);
    }
    else
        interactionEvent(e);
}

void Canvas::mouseReleaseEvent(MouseEvent* e) {
    pickingContainer_->setPickableSelected(false);
    interactionEvent(e);
}

void Canvas::mouseMoveEvent(MouseEvent* e) {
    if(pickingContainer_->isPickableSelected())
        pickingContainer_->movePicked(mousePosToPixelCoordinates(e->pos()));
    else
        interactionEvent(e);
}

void Canvas::keyPressEvent(KeyboardEvent* e) {
	interactionEvent(e);
}

void Canvas::keyReleaseEvent(KeyboardEvent* e) {
	interactionEvent(e);
}

uvec2 Canvas::mousePosToPixelCoordinates(ivec2 mpos){
    uvec2 pos(mpos.x, mpos.y);
    pos.y = dimensions_.y-(pos.y-1);
    return pos;
}

} // namespace
