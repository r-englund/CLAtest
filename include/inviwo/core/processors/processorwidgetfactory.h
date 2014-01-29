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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_PROCESSORWIDGETFACTORY_H
#define IVW_PROCESSORWIDGETFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/processors/processorwidget.h>
#include <inviwo/core/util/factory.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_CORE_API ProcessorWidgetFactory : public Singleton<ProcessorWidgetFactory> {

public:
    ProcessorWidgetFactory();
    ~ProcessorWidgetFactory();

    void registerObject(std::pair<std::string , ProcessorWidget* >);
    ProcessorWidget* create(std::string processorClassName) const;
    ProcessorWidget* create(Processor* processor) const;
    bool isValidType(std::string className) const;

    typedef std::map<std::string, ProcessorWidget*> ProcessorWidgetMap;
private:    
    mutable ProcessorWidgetMap processorWidgetMap_;
};

} // namespace

#endif // IVW_PROCESSORWIDGETFACTORY_H
