#ifndef IVW_OPENGL_QT_MODULE_H
#define IVW_OPENGL_QT_MODULE_H

#include <modules/openglqt/openglqtmoduledefine.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo {

class IVW_MODULE_OPENGL_QT_API OpenglQtModule : public InviwoModule {

public:
    OpenglQtModule();

protected:
    void setupGlobalSystemSettings();
    template <typename T>
    void addProcessorWidgetAndAssociate(ProcessorWidget* processorWidget) {      
        addProcessorWidget(T::CLASS_NAME, processorWidget);       
    }
};

} // namespace

#endif // IVW_OPENGL_QT_MODULE_H
