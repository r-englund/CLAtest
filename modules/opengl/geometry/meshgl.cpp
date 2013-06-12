#include <modules/opengl/geometry/meshgl.h>

namespace inviwo {

MeshGL::MeshGL()
    : GeometryGL()
{
    elementBuffer_ = NULL;
    setRenderer();
}

MeshGL::~MeshGL() {
    deinitialize();
}

void MeshGL::initialize() {}

void MeshGL::deinitialize() {
    for (std::vector<AttributeBufferGL*>::iterator it = arrayBuffers_.begin() ; it != arrayBuffers_.end(); ++it)
        delete (*it);

    delete elementBuffer_;
}

void MeshGL::render(){
    std::vector<AttributeBufferGL*>::iterator it;
    for (it = arrayBuffers_.begin() ; it != arrayBuffers_.end(); ++it)
        (*it)->enable();

    (this->*drawFunc)();

    for (it = arrayBuffers_.begin() ; it != arrayBuffers_.end(); ++it)
        (*it)->disable();
}

void MeshGL::createArrayBuffer(const AttributesBase* attrib){
    AttributeBufferGL* ab = new AttributeBufferGL(attrib);
    ab->upload();
    arrayBuffers_.push_back(ab);
    setRenderer();
}

void MeshGL::createElementBuffer(const AttributesBase* attrib){
    delete elementBuffer_;
    elementBuffer_ = new AttributeBufferGL(attrib, GL_ELEMENT_ARRAY_BUFFER, true);
    elementBuffer_->upload();
    setRenderer();
}

void MeshGL::setRenderer(bool element){
    if(element && elementBuffer_)
        drawFunc = &MeshGL::renderElements;
    else if(arrayBuffers_.size()>0)
        drawFunc = &MeshGL::renderArray;
    else
        drawFunc = &MeshGL::emptyFunc;
}

void MeshGL::renderArray(){
    glDrawArrays(GL_TRIANGLE_STRIP, 0, arrayBuffers_[0]->getAttribute()->getNumberOfAttributes());
}

void MeshGL::renderElements(){
    elementBuffer_->enable();
    glDrawElements(GL_TRIANGLE_STRIP, elementBuffer_->getAttribute()->getNumberOfAttributes(), elementBuffer_->getFormatType(), 0);
}

MeshRAM2GLConverter::MeshRAM2GLConverter()
: RepresentationConverterType<GeometryGL>()
{}

MeshRAM2GLConverter::~MeshRAM2GLConverter() {}

DataRepresentation* MeshRAM2GLConverter::convert(DataRepresentation* source) {     
    MeshRAM* meshRAM = dynamic_cast<MeshRAM*>(source);
    if (meshRAM) {
        MeshGL* meshGL = new MeshGL();
        for (size_t i=0; i < meshRAM->getNumberOfAttributes(); ++i) {
            meshGL->createArrayBuffer(meshRAM->getAttribute(i));
        }
        if(meshRAM->getIndicies()){
            meshGL->createElementBuffer(meshRAM->getIndicies());
        }
        return meshGL;
    }
    return NULL;
}

} // namespace

