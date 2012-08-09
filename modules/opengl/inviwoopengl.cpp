#include "inviwoopengl.h"

void logGLError(const char* fileName, const char* functionName, int lineNumber) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        const GLubyte* errorString = gluErrorString(err);
        std::ostringstream errorMessage;
        errorMessage << "GL Error (" << fileName << ", " << functionName << ", Ln " << lineNumber << "): ";
        errorMessage << (errorString ? (const char*)errorString : "undefined");
        std::cout << errorMessage.str() << std::endl;
    }
}