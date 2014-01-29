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

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/common/inviwomodule.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/io/serialization/ivwserializable.h>

namespace inviwo {

ProcessorFactory::ProcessorFactory() {}

ProcessorFactory::~ProcessorFactory() {}

void ProcessorFactory::registerObject(ProcessorFactoryObject* processor) {
    if(processorClassMap_.find(processor->getClassName()) == processorClassMap_.end()) {
        processorClassMap_.insert(std::make_pair(processor->getClassName(), processor));
    } else {
        LogWarn("Processor with class name: " << processor->getClassName() << " is already registerd")
    }

}

IvwSerializable* ProcessorFactory::create(std::string className) const {
    std::map<std::string, ProcessorFactoryObject*>::iterator it = processorClassMap_.find(className);
    if (it != processorClassMap_.end())
        return it->second->create();
    else
        return 0;
}

bool ProcessorFactory::isValidType(std::string className) const {
    std::map<std::string, ProcessorFactoryObject*>::iterator it = processorClassMap_.find(className);
    if (it != processorClassMap_.end())
        return true;
    else
        return false;
}

} // namespace
