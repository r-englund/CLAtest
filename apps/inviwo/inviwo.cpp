#ifdef _MSC_VER
    #ifdef _DEBUG
        #pragma comment(linker, "/SUBSYSTEM:CONSOLE")
    #else
        #pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    #endif
#endif

#include <QFile>

#include "inviwomainwindow.h"
#include "inviwosplashscreen.h"
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <moduleregistration.h>

int main(int argc, char** argv) {
#ifdef __unix__
    setenv("XLIB_SKIP_ARGB_VISUALS", "1", 1);
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

    // process last arguments
    if (mainWin.processEndCommandLineArgs())
        return inviwoApp.exec();
    else
        return 0;
}
