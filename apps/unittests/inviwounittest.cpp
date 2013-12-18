

#ifdef _MSC_VER
    #pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif


#include <modules/unittests/unittestsmodule.h>

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwoapplication.h>

#include <moduleregistration.h>

#include <gtest/gtest.h>

using namespace inviwo;

int main(int argc , char ** argv){
    InviwoApplication app(argc, argv, "unittest "+IVW_VERSION, IVW_DIR);
    app.initialize(&inviwo::registerAllModules);

    return inviwo::UnitTestsModule::runAllTests();
}