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
#include <QTextStream>
#include <QPainter>

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

void InviwoSplashScreen::drawContents(QPainter* painter) {
    QSplashScreen::drawContents(painter);
    QString versionLabel;
    QTextStream labelStream(&versionLabel);
    labelStream << "Version " << QString::fromStdString(IVW_VERSION);
    //painter->drawText(160, 220, versionLabel);
    painter->drawText(15, 265, versionLabel);
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