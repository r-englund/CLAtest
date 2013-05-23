#ifndef IVW_PROCESSORFACTORYOBJECT_H
#define IVW_PROCESSORFACTORYOBJECT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

class IVW_CORE_API ProcessorFactoryObject  {

public:
    ProcessorFactoryObject(std::string identifier, std::string category, Processor::CodeState codeState)
        : identifier_(identifier), category_(category), codeState_(codeState) {}
    virtual ~ProcessorFactoryObject() {}

    virtual Processor* create() = 0;

    std::string getIdentifier() const { return identifier_; }
    std::string getCategory() const { return category_; }
    Processor::CodeState getCodeState() const { return codeState_; } 

private:
    std::string identifier_;
    std::string category_;
    Processor::CodeState codeState_;
};

template<typename T>
class IVW_CORE_API ProcessorFactoryObjectTemplate : public ProcessorFactoryObject  {

public:
    ProcessorFactoryObjectTemplate(std::string identifier, std::string category, Processor::CodeState codeState) 
        : ProcessorFactoryObject(identifier, category, codeState) {}
    virtual ~ProcessorFactoryObjectTemplate() {}

    virtual Processor* create() { return static_cast<Processor*>(new T()); }
};

#define addProcessorObject(T) { new ProcessorFactoryObjectTemplate<T>(T::CLASS_NAME, T::CATEGORY, T::CODE_STATE) }

} // namespace

#endif // IVW_PROCESSORFACTORYOBJECT_H
