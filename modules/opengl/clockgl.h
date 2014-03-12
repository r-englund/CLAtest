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
 * Main file author: Daniel Jönsson
 *
 *********************************************************************************/

#ifndef IVW_CLOCK_GL_H
#define IVW_CLOCK_GL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#ifndef GL_VERSION_3_3
#include <inviwo/core/util/clock.h>
#endif
namespace inviwo {

/** \class ClockGL
 *
 * Simple timer for OpenGL. 
 * Uses OpenGL queries if OpenGL 3.3 or higher is supported, 
 * otherwise uses CPU which calls glFinish before stopping time.
 * Usage is simplified by the macros (does nothing unless IVW_PROFILING is defined)
 * IVW_BEGIN_OPENGL_PROFILING
 * and 
 * IVW_END_OPENGL_PROFILING
 */
class IVW_MODULE_OPENGL_API ClockGL {
public:
    ClockGL();
    virtual ~ClockGL();

    /** \brief Start OpenGL timing.
     */
    void start();

    /** \brief Stop OpenGL timing.
     */
    void stop();

    /** \brief Returns time taken for command execution in milliseconds.
     * @note Requires that stop has been called before.
     * @return float Time in milliseconds
     */
    float getElapsedTime() const;
    

private:
#ifdef GL_VERSION_3_3
    GLuint queries_[2];
#else 
    Clock clock_;
#endif
    
};

#if IVW_PROFILING 
#define IVW_BEGIN_OPENGL_PROFILING { ClockGL clockGL; clockGL.start();
#else 
#define IVW_BEGIN_OPENGL_PROFILING  
#endif 
#if IVW_PROFILING 
#define IVW_END_OPENGL_PROFILING \
    clockGL.stop(); \
    LogInfo("Exec time: " << clockGL.getElapsedTime() << " ms"); }
#else 
#define IVW_END_OPENGL_PROFILING
#endif

} // namespace

#endif // IVW_CLOCK_GL_H
