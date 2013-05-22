#include "openglqtinfo.h"
#include <inviwo/core/util/formatconversion.h>
#include <inviwo/core/util/logdistributor.h>
#include <inviwo/core/util/stringconversion.h>

namespace inviwo {

#define OpenglQtInfoNotFound(message) { LogInfo(message << " Info could not be retrieved"); }

OpenglQtInfo::OpenglQtInfo() {}

OpenglQtInfo::~OpenglQtInfo() {}

void OpenglQtInfo::printInfo(){
    //Qt General Info
    LogInfo("Qt Version: " << "0.0.0");
}

} // namespace
