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

#ifndef IVW_INVIWOSPLASHSCREEN_H
#define IVW_INVIWOSPLASHSCREEN_H

#include <QSplashScreen>

namespace inviwo {

class InviwoSplashScreen : public QSplashScreen {
Q_OBJECT
public:

	InviwoSplashScreen();
    ~InviwoSplashScreen();

	virtual void show();
	virtual void showMessage(std::string message);
	virtual void finish(QWidget* mainWindow);

private:
	bool showSplashScreen_;
};

} // namespace

#endif // IVW_INVIWOSPLASHSCREEN_H
