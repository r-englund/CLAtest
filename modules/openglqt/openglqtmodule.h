#ifndef IVW_OPENGLQT_MODULE_H
#define IVW_OPENGLQT_MODULE_H

#include <modules/openglqt/openglqtmoduledefine.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo {

class IVW_MODULE_OPENGLQT_API OpenGLQtModule : public InviwoModule {

public:
    OpenGLQtModule();

protected:
    template <typename T>
    void addProcessorWidgetAndAssociate(ProcessorWidget* processorWidget) {      
        addProcessorWidget(T::CLASS_NAME, processorWidget);       
    }
};

} // namespace

#endif // IVW_OPENGLQT_MODULE_H
