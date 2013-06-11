#include "inviwo/core/io/ivfvolumereader.h"
#include "inviwo/core/datastructures/volume/volumeramprecision.h"
#include "inviwo/core/datastructures/volume/volumetypeclassification.h"

namespace inviwo {

IvfReaderSettings::IvfReaderSettings() 
    : ReaderSettings()    
{}


void IvfReaderSettings::serialize(IvwSerializer& s) const {

}

void IvfReaderSettings::deserialize(IvwDeserializer& d) {

}


///////////////////////////////////////////////

IvfVolumeReader::IvfVolumeReader(const std::string filePath) 
    : VolumeReader()
    , sourceFileAbsolutePath_(filePath)
{}

Data* IvfVolumeReader::readData() {    
    ivwAssert(1!=0, "Not implemented");
    return 0;
}    

} // namespace
