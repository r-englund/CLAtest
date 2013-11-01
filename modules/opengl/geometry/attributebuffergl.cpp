#include <modules/opengl/geometry/attributebuffergl.h>

namespace inviwo {

BufferGL::BufferGL(size_t size, BufferType type, const DataFormatBase* format)
    : BufferRepresentation(size, type, format)
    , glFormat_(getGLFormats()->getGLFormat(format->getId())){
    initialize();
    LGL_ERROR_SUPPRESS;
}

BufferGL::~BufferGL() {
    deinitialize();
}

GLenum BufferGL::getFormatType() const {
    return glFormat_.type;
}

GLuint BufferGL::getId() const {
    return id_;
}

void BufferGL::enable() const {
    glEnableClientState(state_);
    bind();
    specifyLocation();
}

void BufferGL::disable() const {
    glDisableClientState(state_);
}

void BufferGL::bind() const {
    glBindBuffer(target_, id_);
}

void BufferGL::specifyLocation() const {
    (this->*locationPointerFunc_)();
}


void BufferGL::upload( const void* data, size_t size, GLenum usage, GLenum target)
{
    //Set global variables
    target_ = target;

    //Get GL Format
    glFormat_ = getGLFormats()->getGLFormat(getDataFormat()->getId());

    //Specify location and state
    switch(getBufferType())
    {
    case COLOR_ATTRIB:
        locationPointerFunc_ = &BufferGL::colorPointer;
        state_ = GL_COLOR_ARRAY;
        break;
    case NORMAL_ATTRIB:
        locationPointerFunc_ = &BufferGL::normalPointer;
        state_ = GL_NORMAL_ARRAY;
        break;
    case TEXCOORD_ATTRIB:
        locationPointerFunc_ = &BufferGL::texCoordPointer;
        state_ = GL_TEXTURE_COORD_ARRAY;
        break;
    case POSITION_ATTRIB:
        locationPointerFunc_ = &BufferGL::vertexPointer;
        state_ = GL_VERTEX_ARRAY;
        break;
    default:
        locationPointerFunc_ = &BufferGL::emptyFunc;
        state_ = GL_VERTEX_ARRAY;
        break;
    }

    reupload(data, size, usage);
}

void BufferGL::reupload(const void* data, size_t size, GLenum usage){
    bind();
    glBufferData(target_, size, data, usage);
    specifyLocation();
}

void BufferGL::colorPointer() const {
    glColorPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void BufferGL::normalPointer() const {
    glNormalPointer(glFormat_.type, 0, 0);
}

void BufferGL::texCoordPointer() const {
    glTexCoordPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void BufferGL::vertexPointer() const {
    glVertexPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void BufferGL::emptyFunc() const {}

BufferGL* BufferGL::clone() const{
    return new BufferGL(*this);
}

void BufferGL::initialize(){
    //Generate a new buffer
    glGenBuffers(1, &id_);
}

void BufferGL::deinitialize(){
    glDeleteBuffers(1, &id_);
}

void BufferGL::download( void* data ) const
{
    bind();
    // Map data
    void* gldata = glMapBuffer(target_, GL_READ_ONLY);
    // Copy data if valid pointer
    if(gldata)
    {
        memcpy(data, gldata, getSize()*getSizeOfElement());
        // Unmap buffer after using it
        glUnmapBufferARB(target_); 
    } else {
        LogError("Unable to map data");
    }

}




} // namespace

