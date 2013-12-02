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

ProcessorFactory::ProcessorFactory() {
    initialize();
}

ProcessorFactory::~ProcessorFactory() {}

void ProcessorFactory::initialize() {
    //TODO: check that inviwoapp is initialized
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
        std::vector<ProcessorFactoryObject*> curProcessorList = inviwoApp->getModules()[curModuleId]->getProcessors();
        for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++)
            registerFactoryObject(curProcessorList[curProcessorId]);
    }
}

void ProcessorFactory::registerFactoryObject(ProcessorFactoryObject* processor) {
    if (processorClassMap_.find(processor->getClassName()) == processorClassMap_.end())
        processorClassMap_.insert(std::make_pair(processor->getClassName(), processor));
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

void ProcessorFactory::deinitialize() {
}

} // namespace
