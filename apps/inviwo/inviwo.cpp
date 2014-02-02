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
#include <inviwo/core/util/filesystem.h>
#include <inviwo/core/util/logcentral.h>
#include "inviwomainwindow.h"
#include "inviwosplashscreen.h"
#include <moduleregistration.h>

int main(int argc, char** argv) {
#ifdef __unix__
    setenv("XLIB_SKIP_ARGB_VISUALS", "1", 1);
#endif
    std::string basePath = inviwo::filesystem::findBasePath();
    inviwo::FileLogger fileLogger(basePath);
    inviwo::LogCentral::instance()->registerLogger(&fileLogger);

    inviwo::InviwoApplicationQt inviwoApp("Inviwo "+IVW_VERSION, basePath, argc, argv);
	
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

    inviwoApp.setWindowIcon(QIcon(":/icons/inviwo_light.png"));

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
        return  0;
}
