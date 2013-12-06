/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
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

#include <inviwo/core/ports/inport.h>

#include <inviwo/core/processors/processor.h>

namespace inviwo {

Inport::Inport(std::string identifier)
: Port(identifier)
{}

Inport::~Inport() {}

std::vector<Processor*> Inport::getPredecessors() {
    std::vector<Processor*> predecessorsProcessors;
    getPredecessorsUsingPortType<Inport>(predecessorsProcessors);
    return predecessorsProcessors;
}






} // namespace
