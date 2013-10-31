#include <modules/openglqt/openglqtmodule.h>
#include <modules/openglqt/openglqtcapabilities.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <modules/opengl/canvasprocessorgl.h>
#include <modules/openglqt/processors/canvasprocessorwidgetqt.h>

namespace inviwo {

OpenglQtModule::OpenglQtModule() : InviwoModule() {
    setIdentifier("OpenglQt");
    setXMLFileName("openglqt/openglqtmodule.xml");   

    addProcessorWidgetAndAssociate<CanvasProcessorGL>(new CanvasProcessorWidgetQt());
    addCapabilities(new OpenglQtCapabilities());
}

} // namespace
