// Automatically generated file do not change! 
#include "inviwo/core/inviwoapplication.h" 
#include "modules/opengl/openglmodule.h"
#include "modules/base/basemodule.h"
namespace inviwo { 
void registerAllModules(InviwoApplication* app) { 
(*app).registerModule(new OpenGLModule());
(*app).registerModule(new BaseModule());
}}
