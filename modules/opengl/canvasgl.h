/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

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

    static inline void enableDrawImagePlaneRect() {
        glEnableClientState(GL_VERTEX_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, screenAlignedVerticesId_);
        glVertexPointer(2, GL_FLOAT, 0, 0);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, screenAlignedTexCoordsId_);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
    }

    static inline void disableDrawImagePlaneRect() {
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    static inline void singleDrawImagePlaneRect() {
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    static inline void multiDrawImagePlaneRect(int instances) {
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, instances);
    }

    static inline void renderImagePlaneRect() {
        enableDrawImagePlaneRect();
        singleDrawImagePlaneRect();
        disableDrawImagePlaneRect();
    }

    static inline void renderImagePlaneRect(int instances) {
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

    void checkChannels(int);

private:
    static bool glewInitialized_;
    static GLuint screenAlignedVerticesId_;
    static GLuint screenAlignedTexCoordsId_;

    const ImageGL* imageGL_;
    LayerType layerType_;
    Shader* shader_;
    Shader* noiseShader_;
    int singleChannel_;

};

} // namespace

#endif // IVW_CANVASGL_H
