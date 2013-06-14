#include <modules/opengl/geometry/meshgl.h>

namespace inviwo {

MeshGL::MeshGL()
    : GeometryGL()
{
    defaultElementBuffer_ = NULL;
    setRenderer();
}

MeshGL::~MeshGL() {
    deinitialize();
}

void MeshGL::initialize() {}

void MeshGL::deinitialize() {
    for (std::vector<AttributeBufferGL*>::iterator it = arrayBuffers_.begin() ; it != arrayBuffers_.end(); ++it)
        delete (*it);

    for (std::vector<std::pair<Connectivity, AttributeBufferGL*>>::iterator it = elementBuffers_.begin() ; it != elementBuffers_.end(); ++it)
        delete it->second;
}

void MeshGL::render() const{
    std::vector<AttributeBufferGL*>::const_iterator it;
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

void MeshGL::createElementBuffer(const AttributesBase* attrib, Connectivity ct){
    AttributeBufferGL* element = new AttributeBufferGL(attrib, GL_ELEMENT_ARRAY_BUFFER, true);
    elementBuffers_.push_back(std::make_pair(ct, element));
    element->upload();
    setRenderer();
}

void MeshGL::setRenderer(bool element){
    if(element && elementBuffers_.size()>0){
        drawFunc = &MeshGL::renderElements;
        defaultElementBuffer_ = elementBuffers_[0].second;
        switch(elementBuffers_[0].first)
        {
        case STRIP:
              elementDrawMode_ = GL_TRIANGLE_STRIP;
              break;
        case FAN:
              elementDrawMode_ = GL_TRIANGLE_FAN;
              break;
        default:
              elementDrawMode_ = GL_TRIANGLES;
        }
    }
    else if(arrayBuffers_.size()>0)
        drawFunc = &MeshGL::renderArray;
    else
        drawFunc = &MeshGL::emptyFunc;
}

void MeshGL::renderArray() const{
    glDrawArrays(elementDrawMode_, 0, arrayBuffers_[0]->getAttribute()->getNumberOfAttributes());
}

void MeshGL::renderElements() const{
    defaultElementBuffer_->enable();
    glDrawElements(elementDrawMode_, defaultElementBuffer_->getAttribute()->getNumberOfAttributes(), defaultElementBuffer_->getFormatType(), 0);
}

MeshRAM2GLConverter::MeshRAM2GLConverter()
: RepresentationConverterType<GeometryGL>()
{}

MeshRAM2GLConverter::~MeshRAM2GLConverter() {}

DataRepresentation* MeshRAM2GLConverter::createFrom(const DataRepresentation* source) {     
    const MeshRAM* meshRAM = dynamic_cast<const MeshRAM*>(source);
    if (meshRAM) {
        MeshGL* meshGL = new MeshGL();
        for (size_t i=0; i < meshRAM->getNumberOfAttributes(); ++i) {
            meshGL->createArrayBuffer(meshRAM->getAttributes(i));
        }
        for (size_t i=0; i < meshRAM->getNumberOfIndicies(); ++i) {
            meshGL->createElementBuffer(meshRAM->getIndicies(i), meshRAM->getIndexConnectivity(i));
        }
        return meshGL;
    }
    return NULL;
}
void MeshRAM2GLConverter::update(const DataRepresentation* source, DataRepresentation* destination) {
    const MeshRAM* meshSrc = dynamic_cast<const MeshRAM*>(source);
    MeshGL* meshDst = dynamic_cast<MeshGL*>(destination);
    if(meshSrc && meshDst) {
        // FIXME: Implement update operation
    }

}

} // namespace

