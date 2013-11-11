#include "inviwosplashscreen.h"

#include <QSplashScreen>

#include <inviwo/core/util/commandlineparser.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>

namespace inviwo { 

InviwoSplashScreen::InviwoSplashScreen()
	: QSplashScreen(QPixmap(":/images/splashscreen.png"), Qt::WindowStaysOnTopHint)
{
	const CommandLineParser* cmdparser = (inviwo::InviwoApplicationQt::getRef()).getCommandLineParser();
	showSplashScreen_ = cmdparser->getShowSplashScreen();
}

InviwoSplashScreen::~InviwoSplashScreen() {}

void InviwoSplashScreen::show() {
	if (showSplashScreen_)
		QSplashScreen::show();
}

void InviwoSplashScreen::showMessage(std::string message) {
	// show message and add whitespace to match layout
	if (showSplashScreen_)
		QSplashScreen::showMessage(QString::fromStdString("  "+message), Qt::AlignLeft|Qt::AlignBottom, Qt::white);
}

void InviwoSplashScreen::finish(QWidget* mainWindow) {
	if (showSplashScreen_)
		QSplashScreen::finish(mainWindow);
}

} // namespace