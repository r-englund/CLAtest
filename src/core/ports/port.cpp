#include <inviwo/core/ports/port.h>

#include <inviwo/core/processors/processor.h>

namespace inviwo {

Port::Port(std::string identifier)
    : identifier_(identifier)
{}

Port::~Port() {}

uvec3 Port::getColorCode() const { 
    return uvec3(128,128,128); 
}

Processor* Port::getProcessor() const { 
    return processor_; 
}

std::string Port::getIdentifier() const {
    return identifier_; 
}

void Port::setIdentifier(const std::string& name) {
    identifier_ = name;
}

void Port::setProcessor(Processor* processor) { 
    processor_ = processor; 
}

void Port::invalidate() { 
    processor_->invalidate(); 
}

} // namespace
