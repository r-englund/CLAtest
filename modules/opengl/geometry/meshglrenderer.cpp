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
#include <modules/opengl/geometry/meshglrenderer.h>

namespace inviwo {



MeshGLRenderer::MeshGLRenderer( const Mesh* mesh ): meshToRender_(mesh)
{
    initialize(mesh->getAttributesInfo());
}

MeshGLRenderer::MeshGLRenderer( const Mesh* mesh, Mesh::AttributesInfo ai): meshToRender_(mesh)
{
    initialize(ai);
}

MeshGLRenderer::MeshGLRenderer( const Mesh* mesh, RenderType rt, ConnectivityType ct ): meshToRender_(mesh)
{
    initialize(Mesh::AttributesInfo(rt, ct));
}

MeshGLRenderer::~MeshGLRenderer()
{

}

void MeshGLRenderer::render( RenderType rt ) const{
    std::vector<Buffer*>::const_iterator it;
    for (it = meshToRender_->getBuffers().begin() ; it != meshToRender_->getBuffers().end(); ++it) {
        (*it)->getRepresentation<BufferGL>()->enable();
    }
    glPushMatrix();
    mat4 modelToWorld = meshToRender_->getWorldMatrix();
    glMultMatrixf(glm::value_ptr(modelToWorld));
    mat4 dataToModel = meshToRender_->getBasisAndOffset();
    glMultMatrixf(glm::value_ptr(dataToModel));
    
    (this->*drawMethods_[rt].drawFunc)(rt);
    
    glPopMatrix();
    for (it = meshToRender_->getBuffers().begin() ; it != meshToRender_->getBuffers().end(); ++it) {
        (*it)->getRepresentation<BufferGL>()->disable();
    }
}

GLenum MeshGLRenderer::getDrawMode( RenderType rt, ConnectivityType ct)
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

void MeshGLRenderer::renderArray( RenderType rt) const
{
    glDrawArrays(drawMethods_[rt].drawMode, 0, static_cast<GLsizei>(meshToRender_->getAttributes(0)->getSize()));
}

void MeshGLRenderer::renderElements( RenderType rt) const
{
    const ElementBufferGL* elementBufferGL = drawMethods_[rt].elementBuffer->getRepresentation<ElementBufferGL>();
    elementBufferGL->enable();
    glDrawElements(drawMethods_[rt].drawMode, static_cast<GLsizei>(elementBufferGL->getSize()), elementBufferGL->getFormatType(), 0);
}

void MeshGLRenderer::initialize( Mesh::AttributesInfo ai )
{
    drawMethods_[0].drawFunc = &MeshGLRenderer::emptyFunc;
    drawMethods_[0].drawMode = getDrawMode(ai.rt, ai.ct);
    drawMethods_[0].elementBuffer = NULL;

    for(int i=1; i<NUMBER_OF_RENDER_TYPES; i++){
        drawMethods_[i].drawFunc = drawMethods_[0].drawFunc;
        drawMethods_[i].drawMode = drawMethods_[0].drawMode;
        drawMethods_[i].elementBuffer = drawMethods_[0].elementBuffer;
    }

    drawMethods_[NOT_SPECIFIED].drawFunc = &MeshGLRenderer::renderArray;
    drawMethods_[POINTS].drawFunc = &MeshGLRenderer::renderArray;
    drawMethods_[POINTS].drawMode = GL_POINTS;

    for (size_t i=0; i < meshToRender_->getNumberOfIndicies(); ++i) {
        if(meshToRender_->getIndicies(i)->getSize() > 0)
            initializeIndexBuffer( meshToRender_->getIndicies(i), meshToRender_->getIndexAttributesInfo(i));
    }
}
void MeshGLRenderer::initializeIndexBuffer( const Buffer* indexBuffer, Mesh::AttributesInfo ai ) {
    drawMethods_[ai.rt].drawFunc = &MeshGLRenderer::renderElements;
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

