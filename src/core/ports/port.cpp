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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include <inviwo/core/ports/port.h>

#include <inviwo/core/processors/processor.h>

namespace inviwo {

uvec3 Port::colorCode = uvec3(128,128,128); 

Port::Port(std::string identifier)
    : identifier_(identifier),
      processor_(0)
{}

Port::~Port() {}

uvec3 Port::getColorCode() const { 
    return Port::colorCode;
}

bool Port::equalColorCode(Port* p) const {
    if(p)
        return (p->getColorCode() == getColorCode());
    else
        return false;
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



void Port::serialize(IvwSerializer& s) const {
    s.serialize("identifier", identifier_, true);
    s.serialize("Processor", processor_);
}

void Port::deserialize(IvwDeserializer& d) {
    d.deserialize("identifier", identifier_, true);
    d.deserialize("Processor", processor_);
}

void Port::invalidate( PropertyOwner::InvalidationLevel invalidationLevel ) {
    processor_->invalidate(invalidationLevel);
}

} // namespace
