#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/util/logdistributor.h>

void LogGLError(const char* fileName, const char* functionName, int lineNumber) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        const GLubyte* errorString = gluErrorString(err);
        std::ostringstream errorMessage;
        errorMessage << "OpenGL Error (" << fileName << ", " << functionName << ", Ln " << lineNumber << "): ";
        errorMessage << (errorString ? (const char*)errorString : "undefined");
        inviwo::LogCentral::instance()->log("OpenGL", inviwo::Error, __FILE__, __FUNCTION__, __LINE__, (errorMessage.str()));
    }
}