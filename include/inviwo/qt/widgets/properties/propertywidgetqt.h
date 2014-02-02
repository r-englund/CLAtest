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
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

#ifndef IVW_PROPERTYWIDGETQT_H
#define IVW_PROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QMenu>
#include <inviwo/core/properties/propertyvisibility.h>
#include <inviwo/core/properties/propertywidget.h>
#include <inviwo/core/util/observer.h>

namespace inviwo {

class Property;

class IVW_QTWIDGETS_API PropertyWidgetQt : public QWidget, public PropertyWidget, public VoidObserver {

    Q_OBJECT

public:
    PropertyWidgetQt();
    PropertyWidgetQt(Property* property);
    virtual ~PropertyWidgetQt();
    PropertyWidgetQt* create();

    void showWidget();
    void hideWidget();
    //void notify();
    void updateContextMenu();
public slots:
    void visibilityModified(int mode);
    void showContextMenu(const QPoint& pos);
    void setDeveloperViewMode(bool value);
    void setApplicationViewMode(bool value);
    void moduleAction();

signals:
    void modified();
    void visibilityChange();
private:
    QMenu* contextMenu_;
    QMap<QString,QMenu*> moduleSubMenus_;
    QActionGroup* viewModeActionGroup_;
    QMenu* viewModeItem_;
protected:
    void setProperty(Property* property);
    PropertyVisibilityMode getApplicationViewMode();
    QMenu* getContextMenu() {return contextMenu_;}
    void generateContextMenu();
    QMenu* generatePropertyWidgetMenu();
    void generateModuleMenuActions();
    void updateModuleMenuActions();

    QAction* developerViewModeAction_;
    QAction* applicationViewModeAction_;

};

} // namespace

#endif // IVW_PROPERTYWIDGETQT_H