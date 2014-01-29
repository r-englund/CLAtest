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

ProcessorWidgetFactory::ProcessorWidgetFactory() {}

ProcessorWidgetFactory::~ProcessorWidgetFactory() {}

void ProcessorWidgetFactory::registerObject(std::pair<std::string, ProcessorWidget* > widget) {
    if(processorWidgetMap_.find(widget.first) == processorWidgetMap_.end()) {
        processorWidgetMap_.insert(widget);
    } else {
        LogWarn("Processor Widget for class name: " << widget.first << " is already registerd");
    }
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


} // namespace
