#include <modules/unittests/unittestsmodule.h>

#include <gtest/gtest.h>

#include <inviwo/core/common/inviwoapplication.h>

using namespace inviwo;

int global_argc;
char** global_argv;
#include <unittests.h>

namespace inviwo{
UnitTestsModule::UnitTestsModule() : InviwoModule() { 
	setIdentifier("UnitTests");
	setXMLFileName("unittests/unittestsmodule.xml");
}

UnitTestsModule::~UnitTestsModule() { } 

void UnitTestsModule::initialize() {
	InviwoModule::initialize(); //call superclass initialize
}
void UnitTestsModule::deinitialize() { }


int UnitTestsModule::runAllTests(){ 
    global_argc = InviwoApplication::getPtr()->getCommandLineParser()->getARGC();
    global_argv = InviwoApplication::getPtr()->getCommandLineParser()->getARGV();
    ::testing::InitGoogleTest(&global_argc, global_argv);
    int res = RUN_ALL_TESTS();
    if(res){
        LogErrorCustom("UnitTestsModule::runAllTests","Some unit tests did not pass, see console output for details");
    }
    return res;
}

} // namespace

