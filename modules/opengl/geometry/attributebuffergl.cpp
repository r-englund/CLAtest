#include <modules/opengl/geometry/attributebuffergl.h>

namespace inviwo {

AttributeBufferGL::AttributeBufferGL()
{
    //Generate a new buffer
    glGenBuffers(1, &id_);
}

AttributeBufferGL::~AttributeBufferGL() {
    glDeleteBuffers(1, &id_);
}

const AttributesBase* AttributeBufferGL::getAttribute() const {
    return attrib_;
}

GLenum AttributeBufferGL::getFormatType() const {
    return glFormat_.type;
}

void AttributeBufferGL::enable() const {
    (this->*enableFunc_)();
}

void AttributeBufferGL::disable() const {
    (this->*disableFunc_)();
}

void AttributeBufferGL::bind() const {
    glBindBuffer(target_, id_);
}

void AttributeBufferGL::specifyLocation() const {
    (this->*locationPointerFunc_)();
}

void AttributeBufferGL::upload(const AttributesBase* attrib, GLenum usage, GLenum target, bool element){
    //Set global variables
    attrib_ = attrib;
    target_ = target;

    //Get GL Format
    glFormat_ = getGLFormats()->getGLFormat(attrib->getDataFormat().getId());

    //Specify location and state
    switch(attrib->getAttributeType())
    {
    case COLOR_ATTRIB:
        locationPointerFunc_ = &AttributeBufferGL::colorPointer;
        state_ = GL_COLOR_ARRAY;
        break;
    case NORMAL_ATTRIB:
        locationPointerFunc_ = &AttributeBufferGL::normalPointer;
        state_ = GL_NORMAL_ARRAY;
        break;
    case TEXCOORD_ATTRIB:
        locationPointerFunc_ = &AttributeBufferGL::texCoordPointer;
        state_ = GL_TEXTURE_COORD_ARRAY;
        break;
    case POSITION_ATTRIB:
        locationPointerFunc_ = &AttributeBufferGL::vertexPointer;
        state_ = GL_VERTEX_ARRAY;
        break;
    default:
        locationPointerFunc_ = &AttributeBufferGL::emptyFunc;
        state_ = GL_VERTEX_ARRAY;
        break;
    }

    //Perform special operations if ELEMENT_ARRAY or ARRAY target
    if(element){
        enableFunc_ = &AttributeBufferGL::enableElementArray; 
        disableFunc_ = &AttributeBufferGL::emptyFunc;
    }
    else{
        enableFunc_ = &AttributeBufferGL::enableArray; 
        disableFunc_ = &AttributeBufferGL::disableArray;
    }

    reupload(usage);
}

void AttributeBufferGL::reupload(GLenum usage){
    bind();
    glBufferData(target_, attrib_->getDataSize(), attrib_->getAttributes(), usage);
    specifyLocation();
}

void AttributeBufferGL::enableArray() const {
    glEnableClientState(state_);
    bind();
    specifyLocation();
}

void AttributeBufferGL::disableArray() const {
    glDisableClientState(state_);
}

void AttributeBufferGL::enableElementArray() const {
    bind();
}

void AttributeBufferGL::colorPointer() const {
    glColorPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void AttributeBufferGL::normalPointer() const {
    glNormalPointer(glFormat_.type, 0, 0);
}

void AttributeBufferGL::texCoordPointer() const {
    glTexCoordPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void AttributeBufferGL::vertexPointer() const {
    glVertexPointer(glFormat_.channels, glFormat_.type, 0, 0);
}

void AttributeBufferGL::emptyFunc() const {}

} // namespace

