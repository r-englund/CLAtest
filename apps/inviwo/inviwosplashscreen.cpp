/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
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