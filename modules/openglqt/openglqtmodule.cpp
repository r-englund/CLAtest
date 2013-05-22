#include <modules/openglqt/openglqtmodule.h>
#include <modules/openglqt/openglqtinfo.h>

namespace inviwo {

OpenglQtModule::OpenglQtModule() : InviwoModule() {
    setIdentifier("OpenglQt");
    setXMLFileName("openglqt/openglqtmodule.xml");
    addResourceInfo(new OpenglQtInfo());

	//TODO:
    //add proecessor widget
    //add property widget
    //add editors
}

} // namespace
