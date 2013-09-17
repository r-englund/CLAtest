#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>

namespace inviwo {

Volume::Volume(uvec3 dimensions, DataFormatBase format) : StructuredData<3>(dimensions, format) {
	representations_.clear();
    metaData_.removeAll();
}

Volume::Volume(VolumeRepresentation* in) : StructuredData<3>(in->getDimensions(), in->getDataFormat()) {
    representations_.clear();
    representations_.push_back(in);
    metaData_.removeAll();
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
    return newVolume;
}

void Volume::setOffset(ivec3 offset) {
	SpatialData<3>::setOffset(Vector<3,float>((float)offset.x,
												  (float)offset.y,
												  (float)offset.z));
}

ivec3 Volume::getOffset() const{
	Vector<3, float> offset = SpatialData<3>::getOffset();
	return ivec3((int)offset.x,(int)offset.y,(int)offset.z);
}

uvec3 Volume::getDimension() const{
	return StructuredData<3>::getDimension();
}
void Volume::setDimension(const uvec3& dim){
	StructuredData<3>::setDimension(dim);
}

void Volume::createDefaultRepresentation() const{
	representations_.push_back(new VolumeDisk(getDimension(), getDataFormat()));
}


} // namespace