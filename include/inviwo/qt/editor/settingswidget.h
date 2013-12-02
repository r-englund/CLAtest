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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_SETTINGSWIDGET_H
#define IVW_SETTINGSWIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/qt/widgets/inviwodockwidget.h>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QString>

namespace inviwo {

class IVW_QTEDITOR_API SettingsWidget : public InviwoDockWidget {
    Q_OBJECT
public:
    SettingsWidget(QWidget* parent);
    SettingsWidget(QString title, QWidget* parent);
    ~SettingsWidget();
    virtual void generateWidget();
    virtual void loadSettings();
    virtual void saveSettings();

protected:
    QVBoxLayout* vLayout_;
    QTabWidget* tabWidget_;
};

} // namespace

#endif // IVW_SETTINGSWIDGET_H