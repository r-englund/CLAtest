#include <QApplication>

#include "../../modules/opengl/inviwoopengl.h"
#include "inviwomainwindow.h"
#include "inviwo/qt/widgets/inviwoapplicationqt.h"

int main(int argc, char** argv) {
    QApplication a(argc, argv);

    glewInit();

    inviwo::InviwoApplicationQt inviwoApp("Inviwo", "D:/inviwo");
    inviwoApp.initialize();

    inviwo::InviwoMainWindow mainWin;
    inviwoApp.setMainWindow(&mainWin);
    mainWin.show();

    return a.exec();
}