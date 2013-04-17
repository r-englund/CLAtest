#ifndef IVW_PROCESSORWIDGETFACTORY_H
#define IVW_PROCESSORWIDGETFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/processors/processorwidget.h>
#include <inviwo/core/util/inviwofactorybase.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_CORE_API ProcessorWidgetFactory : public Factory,
                                           public Singleton<ProcessorWidgetFactory> {

public:
    ProcessorWidgetFactory();
    ~ProcessorWidgetFactory();

    virtual void initialize();
    virtual void deinitialize();

    void registerProcessorWidget(ProcessorWidget* processorWidget);
    virtual IvwSerializable* create(std::string className) const;
    virtual bool isValidType(std::string className) const;

private:
    mutable std::map<std::string, ProcessorWidget*> processorWidgetMap_;
};

} // namespace

#endif // IVW_PROCESSORWIDGETFACTORY_H
