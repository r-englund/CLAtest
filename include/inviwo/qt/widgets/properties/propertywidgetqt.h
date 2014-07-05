 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file authors: Alexander Johansson, Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_PROPERTYWIDGETQT_H
#define IVW_PROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QMenu>
#include <inviwo/qt/widgets/inviwodockwidget.h>
#include <inviwo/core/properties/propertyvisibility.h>
#include <inviwo/core/properties/propertywidget.h>
#include <inviwo/core/util/observer.h>

namespace inviwo {

class Property;

class IVW_QTWIDGETS_API PropertyWidgetQt : public QWidget, public PropertyWidget {
    Q_OBJECT

public:
    PropertyWidgetQt();
    PropertyWidgetQt(Property* property);
    virtual ~PropertyWidgetQt();
    PropertyWidgetQt* create();
    
    virtual UsageMode getUsageMode() const;
    virtual bool getVisible() const;
    virtual void setVisible(bool visible);

    virtual void showWidget();
    virtual void hideWidget();

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

    QMenu* getContextMenu();

    static int MINIMUM_WIDTH;
    static int SPACING;
    static int MARGIN;
    static void setSpacingAndMargins(QLayout* layout);
    
public slots:
    virtual void updateContextMenu();
    virtual void resetPropertyToDefaultState();
    virtual void showContextMenu(const QPoint& pos);
    virtual void setDeveloperUsageMode(bool value);
    virtual void setApplicationUsageMode(bool value);
    virtual void changeSemantics(QAction* action);
    
    void moduleAction();
signals:
    void usageModeChanged();
    void updateSemantics(PropertyWidgetQt*);

protected:
    void setProperty(Property* property);
    UsageMode getApplicationUsageMode();

    // Context menu
    void generateContextMenu();
    void generateModuleMenuActions();
    void updateModuleMenuActions();
    virtual void initializeEditorWidgetsMetaData();

    virtual bool event(QEvent *event); //< for custom tooltips.
    virtual std::string getToolTipText();

    void paintEvent(QPaintEvent *pe);
    
    // Actions
    QMenu* usageModeItem_;
    QActionGroup* usageModeActionGroup_;
    QAction* developerUsageModeAction_;
    QAction* applicationUsageModeAction_;

    QMenu* semanicsMenuItem_;
    QActionGroup* semanticsActionGroup_;
    
    
private:
    QMenu* contextMenu_;
    QMap<QString,QMenu*> moduleSubMenus_;

    bool cached_;
};

//PropertyEditorWidget owned by PropertyWidget

class IVW_QTWIDGETS_API PropertyEditorWidgetQt : public InviwoDockWidget, public PropertyEditorWidget {
    Q_OBJECT
public:
    PropertyEditorWidgetQt(std::string widgetName, QWidget* parent);
    virtual ~PropertyEditorWidgetQt();
    virtual void initialize(Property* property);
    virtual void deinitialize();
};


} // namespace

#endif // IVW_PROPERTYWIDGETQT_H