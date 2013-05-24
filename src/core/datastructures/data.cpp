#include <inviwo/core/datastructures/data.h>

namespace inviwo {

Data::Data() {}
Data::~Data() { clearRepresentations(); }

void Data::clearRepresentations() {
    while (hasRepresentations()) {
        delete representations_.back();
        representations_.pop_back();
    }
}

void Data::copyRepresentations( Data* targetData ) const
{
    targetData->representations_.clear();
    for(size_t i=0; i<representations_.size(); i++) {
        targetData->representations_.push_back(representations_[i]->clone());
    }
}

void Data::addRepresentation(DataRepresentation* representation) {
    representations_.push_back(representation);
}

bool Data::hasRepresentations() const {
    return !representations_.empty();
}

void Data::copyMetaData(Data* targetData) const{
    targetData->metaData_.removeAll();
    targetData->metaData_ = metaData_;
}

/*---------------------------------------------------------------*/

Data3D::Data3D() : PARENT() { }

Data3D::Data3D(Data::TYPE3D dim) : PARENT() {
    setDimension(dim);
}

Data3D::~Data3D() {}

uvec3 Data3D::getDimension() const {
    return Data3D::PARENT::getDimension<uvec3,UVec3MetaData>(uvec3(0));
}

void Data3D::setDimension(uvec3 dim) {
     Data3D::PARENT::setDimension<uvec3,UVec3MetaData>(dim);
}


/*---------------------------------------------------------------*/

Data2D::Data2D() : PARENT() {

}

Data2D::Data2D(Data::TYPE2D dim) : PARENT() {
    setDimension(dim);
}

Data2D::~Data2D() {}

uvec2 Data2D::getDimension() const {
    return Data2D::PARENT::getDimension<uvec2,UVec2MetaData>(uvec2(0));
}

void Data2D::setDimension(uvec2 dim) {
    Data2D::PARENT::setDimension<uvec2,UVec2MetaData>(dim);
}


} // namespace
