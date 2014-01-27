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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/common/inviwomodule.h>
#include <inviwo/core/processors/processorwidgetfactory.h>


namespace inviwo {

ProcessorWidgetFactory::ProcessorWidgetFactory() {
    initialize();
}

ProcessorWidgetFactory::~ProcessorWidgetFactory() {
    deinitialize();
}

void ProcessorWidgetFactory::initialize() {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
        std::vector< std::pair<std::string, ProcessorWidget*> > curProcessorList =
            inviwoApp->getModules()[curModuleId]->getProcessorWidgets();
        for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++)
            registerProcessorWidget(curProcessorList[curProcessorId].first,
                                    curProcessorList[curProcessorId].second);
    }
}

void ProcessorWidgetFactory::registerProcessorWidget(std::string processorClassName,
                                                     ProcessorWidget* processorWidget) {

    if (processorWidgetMap_.find(processorClassName) == processorWidgetMap_.end())
        processorWidgetMap_.insert(std::make_pair(processorClassName, processorWidget));
}

ProcessorWidget* ProcessorWidgetFactory::create(std::string processorClassName) const {
    ProcessorWidgetMap::iterator it = processorWidgetMap_.find(processorClassName);
    if (it != processorWidgetMap_.end())
        return it->second->create();
    else
        return 0;
}

ProcessorWidget* ProcessorWidgetFactory::create(Processor* processor) const {
    return ProcessorWidgetFactory::create(processor->getClassName());
}

bool ProcessorWidgetFactory::isValidType(std::string processorClassName) const {
    ProcessorWidgetMap::iterator it = processorWidgetMap_.find(processorClassName);
    if (it != processorWidgetMap_.end())
        return true;
    else
        return false;
}

void ProcessorWidgetFactory::deinitialize() {
}

} // namespace
