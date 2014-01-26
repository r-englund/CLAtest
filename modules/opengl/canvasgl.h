/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
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

#ifndef IVW_CANVASGL_H
#define IVW_CANVASGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/util/canvas.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/image/imagegl.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API CanvasGL : public Canvas {
public:
    CanvasGL(uvec2 dimensions);
    virtual ~CanvasGL();

    virtual void initialize();
    virtual void deinitialize();
    virtual void activate();
    virtual void render(const Image* im, LayerType layerType = COLOR_LAYER);
    virtual void resize(uvec2 size);
    virtual void glSwapBuffers();
    virtual void update();

    static inline void enableDrawImagePlaneRect(){
        glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, screenAlignedVerticesId_);
        glVertexPointer(2, GL_FLOAT, 0, 0);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, screenAlignedTexCoordsId_);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
    }

    static inline void disableDrawImagePlaneRect(){
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    static inline void singleDrawImagePlaneRect(){
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    static inline void multiDrawImagePlaneRect(int instances){
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, instances);
    }

    static inline void renderImagePlaneRect(){
        enableDrawImagePlaneRect();
        singleDrawImagePlaneRect();
        disableDrawImagePlaneRect();
    }

    static inline void renderImagePlaneRect(int instances){
        enableDrawImagePlaneRect();
        multiDrawImagePlaneRect(instances);
        disableDrawImagePlaneRect();
    }

protected:
    void initializeGLEW();
    virtual void initializeSquare();

    void renderLayer();
    void renderNoise();
    void renderTexture(GLint);

private:
    static bool glewInitialized_;
    static GLuint screenAlignedVerticesId_;
    static GLuint screenAlignedTexCoordsId_;

    const ImageGL* imageGL_;
    LayerType layerType_;
    Shader* shader_;
    Shader* noiseShader_;

};

} // namespace

#endif // IVW_CANVASGL_H
