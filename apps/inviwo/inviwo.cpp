#include <QFile>

#include "modules/opengl/inviwoopengl.h"
#include "inviwomainwindow.h"
#include "inviwo/qt/widgets/inviwoapplicationqt.h"

int main(int argc, char** argv) {

    inviwo::InviwoApplicationQt inviwoApp("Inviwo", "C:/inviwo/inviwo", argc, argv);
    inviwoApp.initialize();

    #if (QT_VERSION >= 0x040400)
        QFile styleSheetFile(":/stylesheets/inviwo.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(styleSheetFile.readAll());
        inviwoApp.setStyleSheet(styleSheet);
        styleSheetFile.close();
    #endif

    inviwo::InviwoMainWindow mainWin;
    inviwoApp.setMainWindow(&mainWin);
    inviwoApp.setWindowIcon(QIcon(":/icons/inviwo_tmp.png"));
    mainWin.show();
    mainWin.openLastNetwork();

    return inviwoApp.exec();
}