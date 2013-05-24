#ifndef IVW_PROCESSORWIDGETFACTORY_H
#define IVW_PROCESSORWIDGETFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/processors/processorwidget.h>
#include <inviwo/core/util/inviwofactorybase.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_CORE_API ProcessorWidgetFactory : public Singleton<ProcessorWidgetFactory> {

public:
    ProcessorWidgetFactory();
    ~ProcessorWidgetFactory();

    void initialize();
    void deinitialize();

    void registerProcessorWidget(std::string processorClassName, ProcessorWidget* processorWidget);
    ProcessorWidget* create(std::string processorClassName) const;
    ProcessorWidget* create(Processor* processor) const;
    bool isValidType(std::string className) const;

private:
    mutable std::map<std::string, ProcessorWidget*> processorWidgetMap_;
};

} // namespace

#endif // IVW_PROCESSORWIDGETFACTORY_H
