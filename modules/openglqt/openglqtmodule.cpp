#include <modules/openglqt/openglqtmodule.h>
#include <modules/openglqt/openglqtinfo.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <modules/opengl/canvasprocessorgl.h>
#include <modules/openglqt/processors/canvasprocessorwidgetqt.h>

namespace inviwo {

OpenglQtModule::OpenglQtModule() : InviwoModule() {
    setIdentifier("OpenglQt");
    setXMLFileName("openglqt/openglqtmodule.xml");    
    
    //TODO: Make sure OpenGL module is initialized before this module
    InviwoApplicationQt* app = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());
    QWidget* parent = app->getMainWindow();

    for (size_t curModuleId=0; curModuleId<app->getModules().size(); curModuleId++) {
        std::vector<Processor*> curProcessorList = app->getModules()[curModuleId]->getProcessors();
        for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++)
            if (dynamic_cast<CanvasProcessorGL*>(curProcessorList[curProcessorId])) {
                ProcessorWidgetQt* processorWidgetQt = new CanvasProcessorWidgetQt(parent);
                addProcessorWidget(processorWidgetQt);
                processorWidgetQt->setProcessor(curProcessorList[curProcessorId]);
            }
    }

    addResourceInfo(new OpenglQtInfo());
}

} // namespace
