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

#ifndef IVW_PROCESSORFACTORYOBJECT_H
#define IVW_PROCESSORFACTORYOBJECT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/processors/processorstate.h>
#include <string>

namespace inviwo {

class Processor;

class IVW_CORE_API ProcessorFactoryObject  {

public:
    ProcessorFactoryObject(std::string className, std::string category, CodeState codeState)
        : className_(className), category_(category), codeState_(codeState) {
    }
    virtual ~ProcessorFactoryObject() {}

    virtual Processor* create() = 0;

    std::string getClassName() const { return className_; }
    std::string getCategory() const { return category_; }
    CodeState getCodeState() const { return codeState_; }

private:
    std::string className_;
    std::string category_;
    CodeState codeState_;
};

template<typename T>
class ProcessorFactoryObjectTemplate : public ProcessorFactoryObject  {

public:
    ProcessorFactoryObjectTemplate(std::string className, std::string category, CodeState codeState)
        : ProcessorFactoryObject(className, category, codeState) {}
    virtual ~ProcessorFactoryObjectTemplate() {}

    virtual Processor* create() { return static_cast<Processor*>(new T()); }
};

} // namespace

#endif // IVW_PROCESSORFACTORYOBJECT_H
