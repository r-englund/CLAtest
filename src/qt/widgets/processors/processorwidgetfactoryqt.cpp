#include <inviwo/qt/widgets/processors/processorwidgetfactoryqt.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>

#include <inviwo/core/processors/canvasprocessor.h>

#include <inviwo/qt/widgets/processors/canvasprocessorwidgetqt.h>

namespace inviwo {

ProcessorWidgetFactoryQt::ProcessorWidgetFactoryQt() {}

ProcessorWidgetQt* ProcessorWidgetFactoryQt::create(Processor* processor) {
    //if (dynamic_cast<CameraProperty*>(property))
    //    return new CameraProcessorWidgetQt(static_cast<CameraProperty*>(property));

    InviwoApplicationQt* app = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());
    if (app) {
        QWidget* parent = app->getMainWindow();

        if (dynamic_cast<CanvasProcessor*>(processor))
            return new CanvasProcessorWidgetQt(0, parent);

        return NULL;
    }

    LogWarn("Inviwo Application could not be requested in ProcessorWidgetQt.")
    return NULL;
}

} // namespace
