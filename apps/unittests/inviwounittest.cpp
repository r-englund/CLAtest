

#ifdef _MSC_VER
    #pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif



#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwoapplication.h>

#include <moduleregistration.h>

#include <gtest/gtest.h>

using namespace inviwo;
int global_argc;
char** global_argv;

#include <unittests.h>

int main(int argc , char ** argv){
    ::testing::InitGoogleTest(&argc, argv);
    global_argc = argc;
    global_argv = argv;

    InviwoApplication app(argc, argv, "unittest "+IVW_VERSION, IVW_DIR);
    app.initialize(&inviwo::registerAllModules);

    int testResults = RUN_ALL_TESTS();
    return testResults;
}