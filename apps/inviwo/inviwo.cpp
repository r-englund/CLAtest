/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/


#ifdef _MSC_VER
    #ifdef _DEBUG
        #pragma comment(linker, "/SUBSYSTEM:CONSOLE")
    #else
        #pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    #endif
#endif



#include <QFile>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>

#include "inviwomainwindow.h"
#include "inviwosplashscreen.h"
#include <moduleregistration.h>

#include <inviwo/core/util/msvc-memleak-includes.h>


int main(int argc, char** argv) {
#ifdef __unix__
    setenv("XLIB_SKIP_ARGB_VISUALS", "1", 1);
#endif
#ifdef IVW_ENABLE_MSVC_MEM_LEAK_TEST
    _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE  | _CRTDBG_MODE_DEBUG);
    _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDERR );
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
    inviwo::InviwoApplicationQt inviwoApp("Inviwo "+IVW_VERSION, IVW_DIR, argc, argv);
	
	// initialize and show splash screen
	inviwo::InviwoSplashScreen splashScreen;
	splashScreen.show();
	splashScreen.showMessage("Loading application...");

#if (QT_VERSION >= 0x040400)
    QFile styleSheetFile(":/stylesheets/inviwo.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    inviwoApp.setStyleSheet(styleSheet);
    styleSheetFile.close();
#endif

    inviwo::InviwoMainWindow mainWin;

    // setup core application
    inviwoApp.setMainWindow(&mainWin);

    //Initialize application and register modules
	splashScreen.showMessage("Initializing modules...");
    inviwoApp.initialize(&inviwo::registerAllModules);

    inviwoApp.setWindowIcon(QIcon(":/icons/inviwo_tmp.png"));

    // setup main window
    mainWin.initialize();

	splashScreen.showMessage("Loading workspace...");
    mainWin.initializeWorkspace();
    mainWin.showWindow();

    // open last workspace
    mainWin.openLastWorkspace();
	splashScreen.finish(&mainWin);

#if defined(REG_INVIWOUNITTESTSMODULE) && defined(IVW_RUN_UNITTEST_ON_STARTUP) 
    int res = inviwo::UnitTestsModule::runAllTests();
#endif
    // process last arguments
    if (mainWin.processEndCommandLineArgs())
        return inviwoApp.exec();
    else
        return 0;
}
