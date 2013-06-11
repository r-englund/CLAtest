#include <modules/opengl/geometry/meshgl.h>

namespace inviwo {

MeshGL::MeshGL()
    : GeometryGL()
{}

MeshGL::~MeshGL() {
    deinitialize();
}

void MeshGL::initialize() {}

void MeshGL::deinitialize() {}

void MeshGL::createNewBuffer(AttributesBase* attribute){
    GLuint vboId;

    //Generate a new VBO
    glGenBuffersARB(1, &vboId);

    //Bind VBO
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);

    //Upload to VBO
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, attribute->getDataSize(), attribute->getAttributes(), GL_STATIC_DRAW_ARB);

    //Specify location
    switch(attribute->getAttributeType())
    {
    case COLOR:
        glColorPointer(attribute->getElementSize(), GL_FLOAT, 0, 0);
    	break;
    }

    vertexBufferIds_.push_back(vboId);
}

} // namespace

