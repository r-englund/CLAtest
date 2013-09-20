#include <inviwo/core/util/formats.h>

namespace inviwo {

DataFormatBase* DataFormatBase::instance_[] = {NULL};

DataFormatBase::DataFormatBase() 
    : formatId_(id()), bitsAllocated_(bitsAllocated()), bitsStored_(bitsStored()), formatStr_(str()){}

DataFormatBase::DataFormatBase(DataFormatId t, size_t bA, size_t bS, std::string s) 
    : formatId_(t), bitsAllocated_(bA), bitsStored_(bS), formatStr_(s){}

float DataFormatBase::convertToNormalizedFloat(void*) const { 
    return 0.f; 
}

glm::vec2 DataFormatBase::convertToNormalizedVec2Float(void*) const { 
    return glm::vec2(0.f); 
}

glm::vec3 DataFormatBase::convertToNormalizedVec3Float(void*) const { 
    return glm::vec3(0.f); 
}

glm::vec4 DataFormatBase::convertToNormalizedVec4Float(void*) const { 
    return glm::vec4(0.f); 
}

size_t DataFormatBase::getBitsAllocated() const { 
    return bitsAllocated_; 
}

size_t DataFormatBase::getBitsStored() const { 
    return bitsStored_; 
}

size_t DataFormatBase::getBytesAllocated() const { 
    return static_cast<size_t>(glm::ceil(BITS_TO_BYTES(static_cast<float>(getBitsAllocated())))); 
}

size_t DataFormatBase::getBytesStored() const { 
    return static_cast<size_t>(glm::ceil(BITS_TO_BYTES(static_cast<float>(getBitsStored())))); 
}

std::string DataFormatBase::getString() const { 
    return formatStr_; 
}

DataFormatId DataFormatBase::getId() const { 
    return formatId_; 
}

} // namespace
