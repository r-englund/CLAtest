#include <inviwo/core/util/commandlineparser.h>
#ifndef IVW_COMMANDLINEPARSER_TEST_H
#define IVW_COMMANDLINEPARSER_TEST_H



TEST(CommandLineParserTest,DefaultTest) {
    const int argc = 1;
    char* argv[argc] = {"unittests.exe"};
    CommandLineParser clp(argc,argv);
    clp.initialize();
    clp.parse();
    EXPECT_STREQ("",clp.getOutputPath().c_str());
    EXPECT_STREQ("",clp.getWorkspacePath().c_str());
    EXPECT_STREQ("",clp.getSnapshotName().c_str());
    EXPECT_STREQ("",clp.getScreenGrabName().c_str());
    EXPECT_STREQ("",clp.getPythonScirptName().c_str());
    EXPECT_FALSE(clp.getCaptureAfterStartup());
    EXPECT_FALSE(clp.getScreenGrabAfterStartup());
    EXPECT_FALSE(clp.getRunPythonScriptAfterStartup());
    EXPECT_FALSE(clp.getQuitApplicationAfterStartup());
    EXPECT_FALSE(clp.getLoadWorkspaceFromArg());
    EXPECT_TRUE(clp.getShowSplashScreen());
}


TEST(CommandLineParserTest,InviwoApplicationsParserTest) {
    InviwoApplication* app = InviwoApplication::getPtr();
    const CommandLineParser* clp = app->getCommandLineParser();
    ASSERT_TRUE(clp != 0);
    EXPECT_STREQ("",clp->getOutputPath().c_str());
    EXPECT_STREQ("",clp->getWorkspacePath().c_str());
    EXPECT_STREQ("",clp->getSnapshotName().c_str());
    EXPECT_STREQ("",clp->getScreenGrabName().c_str());
    EXPECT_STREQ("",clp->getPythonScirptName().c_str());
    EXPECT_FALSE(clp->getCaptureAfterStartup());
    EXPECT_FALSE(clp->getScreenGrabAfterStartup());
    EXPECT_FALSE(clp->getRunPythonScriptAfterStartup());
    EXPECT_FALSE(clp->getQuitApplicationAfterStartup());
    EXPECT_FALSE(clp->getLoadWorkspaceFromArg());
    //   EXPECT_TRUE(clp->getShowSplashScreen());
}



TEST(CommandLineParserTest,CommandLineParserTest) {
    const int argc = 3;
    char* argv[argc] = {"unittests.exe","-w","C:/Just/A/Path/"};
    CommandLineParser clp(argc,argv);
    clp.initialize();
    clp.parse();
    EXPECT_STREQ("",clp.getOutputPath().c_str());
    EXPECT_STREQ("C:/Just/A/Path/",clp.getWorkspacePath().c_str());
    EXPECT_STREQ("",clp.getSnapshotName().c_str());
    EXPECT_STREQ("",clp.getScreenGrabName().c_str());
    EXPECT_STREQ("",clp.getPythonScirptName().c_str());
    EXPECT_FALSE(clp.getCaptureAfterStartup());
    EXPECT_FALSE(clp.getScreenGrabAfterStartup());
    EXPECT_FALSE(clp.getRunPythonScriptAfterStartup());
    EXPECT_FALSE(clp.getQuitApplicationAfterStartup());
    EXPECT_TRUE(clp.getLoadWorkspaceFromArg());
    EXPECT_TRUE(clp.getShowSplashScreen());
}





#endif