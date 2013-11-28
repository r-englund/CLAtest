#include "inviwo/core/io/volumereader.h"

namespace inviwo {

VolumeReader::VolumeReader() : DataReaderType<Volume>() {}

VolumeReader::VolumeReader( const VolumeReader& rhs ) : DataReaderType<Volume>(rhs){ }

VolumeReader& VolumeReader::operator=( const VolumeReader& that ){
    if (this != &that) {
        DataReaderType<Volume>::operator=(that);
    }
    return *this;
}


} // namespace
