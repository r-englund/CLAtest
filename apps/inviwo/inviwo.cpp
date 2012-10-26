#include <QFile>

#include "../../modules/opengl/inviwoopengl.h"
#include "inviwomainwindow.h"
#include "inviwo/qt/widgets/inviwoapplicationqt.h"

int main(int argc, char** argv) {

    inviwo::InviwoApplicationQt inviwoApp("Inviwo", "D:/inviwo", argc, argv);
    inviwoApp.initialize();

    #if (QT_VERSION >= 0x040400)
        QFile styleSheetFile("D:/inviwo/resources/stylesheets/inviwo.qss");
        styleSheetFile.open(QFile::ReadOnly);
        QString styleSheet = QLatin1String(styleSheetFile.readAll());
        inviwoApp.setStyleSheet(styleSheet);
    #endif

    //glewInit();

    inviwo::InviwoMainWindow mainWin;
    inviwoApp.setMainWindow(&mainWin);
    mainWin.showMaximized();

    return inviwoApp.exec();
}