#include "modules/glut/glutmodule.h"

#include "modules/glut/canvasglut.h"

namespace inviwo {

GLUTModule::GLUTModule() : InviwoModule() {
    setIdentifier("GLUT");
    setXMLFileName("glut/glutmodule.xml");
}

} // namespace
