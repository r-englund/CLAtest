// Automatically generated file do not change! 
#include "inviwo/core/inviwoapplication.h" 
#include "modules/base/basemodule.h" 
#include "modules/opengl/openglmodule.h" 
#include "modules/glut/glutmodule.h" 
namespace inviwo { 
void registerAllModules(InviwoApplication* app) { 
 (*app).registerModule(new BaseModule()); 
 (*app).registerModule(new OpenGLModule()); 
 (*app).registerModule(new GLUTModule()); 
}} 
