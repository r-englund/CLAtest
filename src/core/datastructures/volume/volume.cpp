#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>

namespace inviwo {

Volume::Volume(uvec3 dimensions, const DataFormatBase* format) : StructuredData<3>(dimensions, format) {
	representations_.clear();
    //metaData_.removeAll(); // why!?! deletes metadata created by parent? /Peter
}

Volume::Volume(VolumeRepresentation* in) : StructuredData<3>(in->getDimensions(), in->getDataFormat()) {
    representations_.clear();
    representations_.push_back(in);
    //metaData_.removeAll();
}

Volume::Volume(VolumeRepresentation* in, const Volume* src) : StructuredData<3>(in->getDimensions(), in->getDataFormat()) {
    representations_.clear();
    representations_.push_back(in);
    this->metaData_ = src->getMetaDataMap();
}

Volume::~Volume() {}

Data* Volume::clone() const {
	Volume* newVolume = new Volume(getDimension(), getDataFormat());
	copyRepresentations(newVolume);
	newVolume->dataFormatBase_ = dataFormatBase_;
	newVolume->metaData_ = metaData_;
    return newVolume;
}


uvec3 Volume::getDimension() const{
    return StructuredData<3>::getDimension();
}
void Volume::setDimension(const uvec3& dim){
    StructuredData<3>::setDimension(dim);
}

void Volume::setOffset(const vec3& offset) {
	SpatialData<3>::setOffset(Vector<3,float>(offset));
}
vec3 Volume::getOffset() const{
    return SpatialData<3>::getOffset();
}

mat3 Volume::getBasis() const {
    return SpatialData<3>::getBasis();
}
void Volume::setBasis(const mat3& basis){
    SpatialData<3>::setBasis(Matrix<3,float>(basis));
}

mat4 Volume::getBasisAndOffset() const{
    return SpatialData<3>::getBasisAndOffset();
}
void Volume::setBasisAndOffset(const mat4& mat){
    SpatialData<3>::setBasisAndOffset(Matrix<4,float>(mat));
}

mat4 Volume::getWorldTransform() const{
    return SpatialData<3>::getWorldTransform();
}
void Volume::setWorldTransform(const mat4& mat){
    SpatialData<3>::SetWorldTransform(Matrix<4,float>(mat));
}

void Volume::createDefaultRepresentation() const{
	representations_.push_back(new VolumeDisk(getDimension(), getDataFormat()));
}


} // namespace