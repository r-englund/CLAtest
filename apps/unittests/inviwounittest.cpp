
#ifdef _MSC_VER
    #pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#define new DEBUG_NEW

#include <modules/unittests/unittestsmodule.h>

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwoapplication.h>

#include <moduleregistration.h>


using namespace inviwo;

int main(int argc , char ** argv){
    _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE  | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    
    int ret = -1;
    inviwo::ConsoleLogger* logger = new inviwo::ConsoleLogger();
    { //scope for ivw app
        // Search for directory containing data folder to find application basepath. 
        // Working directory will be used if data folder is not found in parent directories.
        std::string basePath = inviwo::filesystem::getParentFolderPath(inviwo::filesystem::getWorkingDirectory(), "data");
        InviwoApplication app(argc, argv, "unittest " + IVW_VERSION, basePath);
        app.initialize(&inviwo::registerAllModules);

        inviwo::LogCentral::instance()->registerLogger(logger);

        ret = inviwo::UnitTestsModule::runAllTests();
    }
    delete logger;
    return ret;
}