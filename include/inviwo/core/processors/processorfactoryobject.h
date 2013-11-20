#ifndef IVW_PROCESSORFACTORYOBJECT_H
#define IVW_PROCESSORFACTORYOBJECT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

class IVW_CORE_API ProcessorFactoryObject  {

public:
    ProcessorFactoryObject(std::string className, std::string category, Processor::CodeState codeState)
        : className_(className), category_(category), codeState_(codeState) {}
    virtual ~ProcessorFactoryObject() {}

    virtual Processor* create() = 0;

    std::string getClassName() const { return className_; }
    std::string getCategory() const { return category_; }
    Processor::CodeState getCodeState() const { return codeState_; } 

private:
    std::string className_;
    std::string category_;
    Processor::CodeState codeState_;
};

template<typename T>
class ProcessorFactoryObjectTemplate : public ProcessorFactoryObject  {

public:
    ProcessorFactoryObjectTemplate(std::string className, std::string category, Processor::CodeState codeState) 
        : ProcessorFactoryObject(className, category, codeState) {}
    virtual ~ProcessorFactoryObjectTemplate() {}

    virtual Processor* create() { return static_cast<Processor*>(new T()); }
};

#define createProcessorFactoryObject(T) { new ProcessorFactoryObjectTemplate<T>(T::CLASS_NAME, T::CATEGORY, T::CODE_STATE) }

} // namespace

#endif // IVW_PROCESSORFACTORYOBJECT_H
