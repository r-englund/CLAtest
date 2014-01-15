/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <modules/opengl/buffer/elementbuffergl.h>
#include <modules/opengl/rendering/meshrenderer.h>

namespace inviwo {



MeshRenderer::MeshRenderer(): meshToRender_(NULL) {

}

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

void MeshRenderer::render() {
    render(NOT_SPECIFIED);
}

void MeshRenderer::render(RenderType rt) {
    const MeshGL* meshGL = getMeshGL();
    meshGL->enable();    
    (this->*drawMethods_[rt].drawFunc)(rt);
    meshGL->disable();    
}

const MeshGL* MeshRenderer::getMeshGL() const{
    return meshToRender_->getRepresentation<MeshGL>();
}

GLenum MeshRenderer::getDefaultDrawMode(){
    return drawMethods_[0].drawMode;
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
        case ADJACENCY:
            return GL_TRIANGLES_ADJACENCY;
        case STRIP_ADJACENCY:
            return GL_TRIANGLE_STRIP_ADJACENCY;
        default:
            return GL_POINTS;
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
        case ADJACENCY:
            return GL_LINES_ADJACENCY;
        case STRIP_ADJACENCY:
            return GL_LINE_STRIP_ADJACENCY;
        default:
            return GL_POINTS;
        }
    default:
        return GL_POINTS;
    }
}

void MeshRenderer::renderArray( RenderType rt) const
{
    glDrawArrays(drawMethods_[rt].drawMode, 0, static_cast<GLsizei>(meshToRender_->getAttributes(0)->getSize()));
}

void MeshRenderer::renderElements( RenderType rt) const
{
    const ElementBufferGL* elementBufferGL = drawMethods_[rt].elementBuffer->getRepresentation<ElementBufferGL>();
    elementBufferGL->enable();
    glDrawElements(drawMethods_[rt].drawMode, static_cast<GLsizei>(elementBufferGL->getSize()), elementBufferGL->getFormatType(), 0);
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
    drawMethods_[ai.rt].elementBuffer = indexBuffer;

    // Specify first element buffer as default rendering method
    if(!drawMethods_[NOT_SPECIFIED].elementBuffer) {
        drawMethods_[NOT_SPECIFIED].drawFunc = drawMethods_[ai.rt].drawFunc;
        drawMethods_[NOT_SPECIFIED].drawMode = drawMethods_[ai.rt].drawMode;
        drawMethods_[NOT_SPECIFIED].elementBuffer = drawMethods_[ai.rt].elementBuffer;
    }
}


} // namespace

