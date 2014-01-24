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

#ifndef IVW_INVIWOAPPLICATIONQT_H
#define IVW_INVIWOAPPLICATIONQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/timerqt.h>
#include <QApplication>
#include <QMainWindow>
#include <QFileSystemWatcher>

#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

class IVW_QTWIDGETS_API InviwoApplicationQt : public QApplication, public InviwoApplication {

    Q_OBJECT

public:
    InviwoApplicationQt(std::string displayName_, std::string basePath_,
                        int& argc, char** argv);

    virtual ~InviwoApplicationQt();

    void setMainWindow(QMainWindow* mainWindow);
    QMainWindow* getMainWindow() { return mainWindow_; }

    virtual void registerFileObserver(FileObserver* fileObserver);
    virtual void startFileObservation(std::string fileName);
    virtual void stopFileObservation(std::string fileName);

	virtual void closeInviwoApplication();

    virtual void playSound(unsigned int soundID);

	virtual void initialize(registerModuleFuncPtr);

    virtual Timer* createTimer()const;

public slots:
    void fileChanged(QString fileName);

protected:
    void wait(int);

private:
    QMainWindow* mainWindow_;
    std::vector<FileObserver*> fileObservers_;
    QFileSystemWatcher* fileWatcher_;
};

} // namespace

#endif // IVW_INVIWOAPPLICATIONQT_H
