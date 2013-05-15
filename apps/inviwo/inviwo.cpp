#ifdef _MSC_VER
    #ifdef _DEBUG
        #pragma comment(linker, "/SUBSYSTEM:CONSOLE")
    #elif
        #pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    #endif
#endif

#include <QFile>

#include "inviwomainwindow.h"
#include <inviwo/qt/widgets/inviwoapplicationqt.h>

int main(int argc, char** argv) {
#ifdef __unix__
    setenv("XLIB_SKIP_ARGB_VISUALS", "1", 1);
#endif
    inviwo::InviwoApplicationQt inviwoApp("Inviwo "+IVW_VERSION, IVW_DIR, argc, argv);

#if (QT_VERSION >= 0x040400)
    QFile styleSheetFile(":/stylesheets/inviwo.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    inviwoApp.setStyleSheet(styleSheet);
    styleSheetFile.close();
#endif

    inviwo::InviwoMainWindow mainWin;

    // setup core application
    inviwoApp.initialize();
    inviwoApp.setMainWindow(&mainWin);
    inviwoApp.setWindowIcon(QIcon(":/icons/inviwo_tmp.png"));

    // setup main window
    mainWin.initializeAndShow();
    mainWin.initializeWorkspace();

    // open last network
    mainWin.openLastNetwork();

    // process last arguments
    if(mainWin.processEndCommandLineArgs())
        return inviwoApp.exec();
    else
        return 0;
}
