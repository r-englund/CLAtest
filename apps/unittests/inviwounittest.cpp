
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>


#ifdef _MSC_VER
    #pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif


#include <modules/unittests/unittestsmodule.h>

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwoapplication.h>

#include <moduleregistration.h>


using namespace inviwo;

int main(int argc , char ** argv){
    /*_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE  | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );*/
    
    InviwoApplication app(argc, argv, "unittest "+IVW_VERSION, IVW_DIR);
    app.initialize(&inviwo::registerAllModules);

    inviwo::LogCentral::instance()->registerLogger(new inviwo::ConsoleLogger());
	
    return inviwo::UnitTestsModule::runAllTests();
}