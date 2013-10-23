#include <inviwo/core/util/formats.h>

namespace inviwo {

DataFormatBase* DataFormatBase::instance_[] = {NULL};

DataFormatBase::DataFormatBase() 
    : formatId_(id()), bitsAllocated_(bitsAllocated()), bitsStored_(bitsStored())
{
    formatStr_ = new std::string(str());
}

DataFormatBase::DataFormatBase(DataFormatId t, size_t bA, size_t bS, std::string s) 
    : formatId_(t), bitsAllocated_(bA), bitsStored_(bS)
{
    formatStr_ = new std::string(s);
}

float DataFormatBase::valueToNormalizedFloat(void*) const { 
    return 0.f; 
}

vec2 DataFormatBase::valueToNormalizedVec2Float(void*) const { 
    return vec2(0.f); 
}

vec3 DataFormatBase::valueToNormalizedVec3Float(void*) const { 
    return vec3(0.f); 
}

vec4 DataFormatBase::valueToNormalizedVec4Float(void*) const { 
    return vec4(0.f); 
}

void DataFormatBase::floatToValue(float val, void* loc) const { 
    loc = &val;
}

void DataFormatBase::vec2ToValue(vec2 val, void* loc) const { 
    loc = &val;
}

void DataFormatBase::vec3ToValue(vec3 val, void* loc) const { 
    loc = &val;
}

void DataFormatBase::vec4ToValue(vec4 val, void* loc) const { 
    loc = &val; 
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

const char* DataFormatBase::getString() const { 
    return formatStr_->c_str(); 
}

DataFormatId DataFormatBase::getId() const { 
    return formatId_; 
}

} // namespace
