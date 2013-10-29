#include <modules/opengl/buffer/elementbuffergl.h>
#include <modules/opengl/geometry/meshgl.h>

namespace inviwo {



MeshRenderer::MeshRenderer( const Mesh* mesh ): meshToRender_(mesh)
{
    initialize(mesh->getAttributesInfo());
}

MeshRenderer::MeshRenderer( const Mesh* mesh, Mesh::AttributesInfo ai): meshToRender_(mesh)
{
    initialize(ai);
}

MeshRenderer::MeshRenderer( const Mesh* mesh, RenderType rt, ConnectivityType ct ): meshToRender_(mesh)
{
    initialize(Mesh::AttributesInfo(rt, ct));
}

MeshRenderer::~MeshRenderer()
{

}

void MeshRenderer::render( RenderType rt ) const{
    std::vector<Buffer*>::const_iterator it;
    for (it = meshToRender_->getBuffers().begin() ; it != meshToRender_->getBuffers().end(); ++it) {
        (*it)->getRepresentation<BufferGL>()->enable();
    }
 
    (this->*drawMethods_[rt].drawFunc)(rt);

    for (it = meshToRender_->getBuffers().begin() ; it != meshToRender_->getBuffers().end(); ++it) {
        (*it)->getRepresentation<BufferGL>()->disable();
    }
}

GLenum MeshRenderer::getDrawMode( RenderType rt, ConnectivityType ct)
{
    switch(rt)
    {
    case TRIANGLES:
        switch(ct)
        {
        case NONE:
            return GL_TRIANGLES;
        case STRIP:
            return GL_TRIANGLE_STRIP;
        case FAN:
            return GL_TRIANGLE_FAN;
        }
    case LINES:
        switch(ct)
        {
        case NONE:
            return GL_LINES;
        case STRIP:
            return GL_LINE_STRIP;
        case LOOP:
            return GL_LINE_LOOP;
        }
    default:
        return GL_POINTS;
    }
}

void MeshRenderer::renderArray( RenderType rt) const
{
    glDrawArrays(drawMethods_[rt].drawMode, 0, meshToRender_->getAttributes(0)->getSize());
}

void MeshRenderer::renderElements( RenderType rt) const
{
    const ElementBufferGL* elementBufferGL = drawMethods_[rt].elementBuffer->getRepresentation<ElementBufferGL>();
    glDrawElements(drawMethods_[rt].drawMode, elementBufferGL->getSize(), elementBufferGL->getFormatType(), 0);
}

void MeshRenderer::initialize( Mesh::AttributesInfo ai )
{
    drawMethods_[0].drawFunc = &MeshRenderer::emptyFunc;
    drawMethods_[0].drawMode = getDrawMode(ai.rt, ai.ct);
    drawMethods_[0].elementBuffer = NULL;

    for(int i=1; i<NUMBER_OF_RENDER_TYPES; i++){
        drawMethods_[i].drawFunc = drawMethods_[0].drawFunc;
        drawMethods_[i].drawMode = drawMethods_[0].drawMode;
        drawMethods_[i].elementBuffer = drawMethods_[0].elementBuffer;
    }

    drawMethods_[NOT_SPECIFIED].drawFunc = &MeshRenderer::renderArray;
    drawMethods_[POINTS].drawFunc = &MeshRenderer::renderArray;
    drawMethods_[POINTS].drawMode = GL_POINTS;

    for (size_t i=0; i < meshToRender_->getNumberOfIndicies(); ++i) {
        if(meshToRender_->getIndicies(i)->getSize() > 0)
            initializeIndexBuffer( meshToRender_->getIndicies(i), meshToRender_->getIndexAttributesInfo(i));
    }
}
void MeshRenderer::initializeIndexBuffer( const Buffer* indexBuffer, Mesh::AttributesInfo ai ) {
    drawMethods_[ai.rt].drawFunc = &MeshRenderer::renderElements;
    drawMethods_[ai.rt].drawMode = getDrawMode(ai.rt, ai.ct);
    delete drawMethods_[ai.rt].elementBuffer;
    drawMethods_[ai.rt].elementBuffer = indexBuffer;

    // Specify first element buffer as default rendering method
    if(!drawMethods_[NOT_SPECIFIED].elementBuffer) {
        drawMethods_[NOT_SPECIFIED].drawFunc = drawMethods_[ai.rt].drawFunc;
        drawMethods_[NOT_SPECIFIED].drawMode = drawMethods_[ai.rt].drawMode;
        drawMethods_[NOT_SPECIFIED].elementBuffer = drawMethods_[ai.rt].elementBuffer;
    }
}


} // namespace

