#ifndef IVW_PROCESSORWIDGETFACTORYQT_H
#define IVW_PROCESSORWIDGETFACTORYQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/util/singleton.h>

#include <inviwo/qt/widgets/processors/processorwidgetqt.h>

namespace inviwo {

class IVW_QTWIDGETS_API ProcessorWidgetFactoryQt : public Singleton<ProcessorWidgetFactoryQt> {
//TODO: derive from Factory class

public:
    ProcessorWidgetFactoryQt();
    ProcessorWidgetQt* create(Processor* processor);

    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_PROCESSORWIDGETFACTORYQT_H