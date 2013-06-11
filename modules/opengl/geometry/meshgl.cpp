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
    GLFormats::GLFormat glFormat = getGLFormats()->getGLFormat(attribute->getDataFormat().getId());

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
        glColorPointer(glFormat.channels, glFormat.type, 0, attribute->getAttributes());
    	break;
    case NORMAL:
        glNormalPointer(glFormat.type, 0, attribute->getAttributes());
        break;
    case TEXCOORD:
        glTexCoordPointer(glFormat.channels, glFormat.type, 0, attribute->getAttributes());
        break;
    case POSITION:
        glVertexPointer(glFormat.channels, glFormat.type, 0, attribute->getAttributes());
        break;
    }

    vertexBufferIds_.push_back(vboId);
}

} // namespace

