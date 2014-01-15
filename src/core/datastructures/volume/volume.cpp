/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#include <inviwo/core/datastructures/volume/volume.h>
#include <inviwo/core/datastructures/volume/volumedisk.h>

namespace inviwo {

Volume::Volume(uvec3 dimensions, const DataFormatBase* format) : Data(format), StructuredGridMetaData<3>(dimensions) {}

Volume::Volume(const Volume& rhs) : Data(rhs), StructuredGridMetaData<3>(rhs) {}

Volume::Volume(VolumeRepresentation* in) : Data(in->getDataFormat()), StructuredGridMetaData<3>(in->getDimension()) {
    addRepresentation(in);
}

Volume& Volume::operator=(const Volume& that) {
    if(this != &that) {
        Data::operator=(that);
        StructuredGridMetaData<3>::operator=(that);
    }
    return *this;
}
Volume* Volume::clone() const {
    return new Volume(*this);
}
Volume::~Volume() {}

uvec3 Volume::getDimension() const{
    return StructuredGridMetaData<3>::getDimension();
}
void Volume::setDimension(const uvec3& dim){
    StructuredGridMetaData<3>::setDimension(dim);
}

void Volume::setOffset(const vec3& offset) {
	SpatialMetaData<3>::setOffset(Vector<3,float>(offset));
}
vec3 Volume::getOffset() const{
    return SpatialMetaData<3>::getOffset();
}

mat3 Volume::getBasis() const {
    return SpatialMetaData<3>::getBasis();
}
void Volume::setBasis(const mat3& basis){
    SpatialMetaData<3>::setBasis(Matrix<3,float>(basis));
}

mat4 Volume::getBasisAndOffset() const{
    return SpatialMetaData<3>::getBasisAndOffset();
}
void Volume::setBasisAndOffset(const mat4& mat){
    SpatialMetaData<3>::setBasisAndOffset(Matrix<4,float>(mat));
}

mat4 Volume::getWorldTransform() const{
    return SpatialMetaData<3>::getWorldTransform();
}
void Volume::setWorldTransform(const mat4& mat){
    SpatialMetaData<3>::setWorldTransform(Matrix<4,float>(mat));
}

DataRepresentation* Volume::createDefaultRepresentation() {
	return new VolumeDisk(getDimension(), getDataFormat());
}


} // namespace