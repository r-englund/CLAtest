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
 * Main file authors: Sathish Kottravel, Alexander Johansson
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/settings/systemsettings.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/core/common/moduleaction.h>
#include <QDesktopWidget>

#include <QStyleOption>
#include <QPainter>

namespace inviwo {

PropertyWidgetQt::PropertyWidgetQt()
    : PropertyWidget()
    , viewModeItem_(NULL) {
}

PropertyWidgetQt::PropertyWidgetQt(Property* property)
    : PropertyWidget(property)
    , viewModeItem_(NULL) {
    this->setToolTip(property_->getIdentifier().c_str());
}

PropertyWidgetQt::~PropertyWidgetQt() {
}


void PropertyWidgetQt::showWidget() {
    this->show();
    emit visibilityChange();
}

void PropertyWidgetQt::hideWidget() {
    this->hide();
    emit visibilityChange();
}

void PropertyWidgetQt::initializeEditorWidgetsMetaData() {
    if (hasEditorWidget()) {
        //Validates editor widget position
        PropertyEditorWidgetQt* propertyEditorWidget = dynamic_cast<PropertyEditorWidgetQt*>(getEditorWidget());
        InviwoApplicationQt* app = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());

        if (!propertyEditorWidget)
            return;

        //set widget meta data stuff
        PropertyEditorWidgetDockStatus docStatus = propertyEditorWidget->getEditorDockStatus();

        if (docStatus == PropertyEditorWidgetDockStatus::DockedLeft) {
            app->getMainWindow()->addDockWidget(Qt::LeftDockWidgetArea, propertyEditorWidget);
            propertyEditorWidget->setFloating(false);
        }
        else if (docStatus == PropertyEditorWidgetDockStatus::DockedRight) {
            app->getMainWindow()->addDockWidget(Qt::RightDockWidgetArea, propertyEditorWidget);
            propertyEditorWidget->setFloating(false);
        }
        else {
            app->getMainWindow()->addDockWidget(Qt::RightDockWidgetArea, propertyEditorWidget);
            propertyEditorWidget->setFloating(true);
        }

        propertyEditorWidget->hide();
        ivec2 widgetDimension = getEditorWidget()->getEditorDimensionMetaData();
        propertyEditorWidget->resize(QSize(widgetDimension.x, widgetDimension.y));
        //TODO: Desktop screen info should be added to system capabilities
        ivec2 pos = getEditorWidget()->getEditorPositionMetaData();
        QDesktopWidget* desktop = QApplication::desktop();
        int primaryScreenIndex = desktop->primaryScreen();
        QRect wholeScreenGeometry = desktop->screenGeometry(primaryScreenIndex);
        QRect primaryScreenGeometry = desktop->screenGeometry(primaryScreenIndex);

        for (int i=0; i<desktop->screenCount(); i++) {
            if (i!=primaryScreenIndex)
                wholeScreenGeometry = wholeScreenGeometry.united(desktop->screenGeometry(i));
        }

        wholeScreenGeometry.setRect(wholeScreenGeometry.x()-10, wholeScreenGeometry.y()-10,
                                    wholeScreenGeometry.width()+20, wholeScreenGeometry.height()+20);
        QPoint bottomRight = QPoint(pos.x+this->width(), pos.y+this->height());
        QPoint appPos = app->getMainWindow()->pos();

        if (!wholeScreenGeometry.contains(QPoint(pos.x, pos.y)) || !wholeScreenGeometry.contains(bottomRight)) {
            pos = ivec2(appPos.x(), appPos.y()) ;
            pos += ivec2(primaryScreenGeometry.width()/2, primaryScreenGeometry.height()/2);
            propertyEditorWidget->move(pos.x, pos.y);
        }
        else {
            if (!(pos.x == 0 && pos.y == 0))
                propertyEditorWidget->move(pos.x, pos.y);
            else {
                pos = ivec2(appPos.x(), appPos.y()) ;
                pos += ivec2(primaryScreenGeometry.width()/2, primaryScreenGeometry.height()/2);
                propertyEditorWidget->move(pos.x, pos.y);
            }
        }

        bool visible = getEditorWidget()->getEditorVisibilityMetaData();

        if (!visible) propertyEditorWidget->hide();
        else propertyEditorWidget->show();
    }
}

void PropertyWidgetQt::paintEvent(QPaintEvent* pe) {
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(
        QStyle::PE_Widget, &o, &p, this);
};

void PropertyWidgetQt::visibilityModified(int mode) {}

void PropertyWidgetQt::generateContextMenu() {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    ivwAssert(inviwoApp->isInitialized()!=false, "InviwoApplication not initialized.This should not be the case.");
    this->addObservation(static_cast<OptionPropertyInt*>(inviwoApp->getSettingsByType<SystemSettings>()->getPropertyByIdentifier("viewMode")));
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    contextMenu_ = new QMenu(this);
    viewModeItem_ = new QMenu(tr("&View mode"),contextMenu_);
    developerViewModeAction_ = new QAction(tr("&Developer"),this);
    developerViewModeAction_->setCheckable(true);
    viewModeItem_->addAction(developerViewModeAction_);
    applicationViewModeAction_ = new QAction(tr("&Application"),this);
    applicationViewModeAction_->setCheckable(true);
    viewModeItem_->addAction(applicationViewModeAction_);
    viewModeActionGroup_ = new QActionGroup(this);
    viewModeActionGroup_->addAction(developerViewModeAction_);
    viewModeActionGroup_->addAction(applicationViewModeAction_);
    contextMenu_->addMenu(viewModeItem_);
    generateModuleMenuActions();
    QMapIterator<QString,QMenu*> it(moduleSubMenus_);

    while (it.hasNext()) {
        it.next();
        contextMenu_->addMenu(it.value());
    }

    updateContextMenu();
    connect(this,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint&)));
    connect(developerViewModeAction_,SIGNAL(triggered(bool)),this, SLOT(setDeveloperViewMode(bool)));
    connect(applicationViewModeAction_,SIGNAL(triggered(bool)),this, SLOT(setApplicationViewMode(bool)));
}

void PropertyWidgetQt::showContextMenu(const QPoint& pos) {
    PropertyVisibilityMode appVisibilityMode  = getApplicationViewMode();

    if (appVisibilityMode == DEVELOPMENT) {
        updateContextMenu();
        QPoint globalPos = this->mapToGlobal(pos);
        contextMenu_->exec(globalPos);
    }
}
QMenu* PropertyWidgetQt::generatePropertyWidgetMenu() {
    QMenu* contextMenu = new QMenu(this);

    if (viewModeItem_ != NULL)
        contextMenu->addMenu(viewModeItem_);
    else {
        developerViewModeAction_ = new QAction(tr("&Developer"),this);
        developerViewModeAction_->setCheckable(true);
        viewModeItem_->addAction(developerViewModeAction_);
        applicationViewModeAction_ = new QAction(tr("&Application"),this);
        applicationViewModeAction_->setCheckable(true);
        viewModeItem_->addAction(applicationViewModeAction_);
        viewModeActionGroup_ = new QActionGroup(this);
        viewModeActionGroup_->addAction(developerViewModeAction_);
        viewModeActionGroup_->addAction(applicationViewModeAction_);
        contextMenu->addMenu(viewModeItem_);
    }

    generateModuleMenuActions();
    QMapIterator<QString,QMenu*> it(moduleSubMenus_);

    while (it.hasNext()) {
        it.next();
        contextMenu->addMenu(it.value());
    }

    return contextMenu;
}

void PropertyWidgetQt::generateModuleMenuActions() {
    moduleSubMenus_.clear();
    InviwoApplication* app = InviwoApplication::getPtr();
    std::vector<ModuleCallbackAction*> moduleActions = app->getCallbackActions();
    std::map<std::string, std::vector<ModuleCallbackAction*> > callbackMapPerModule;

    for (size_t i=0; i<moduleActions.size(); i++)
        callbackMapPerModule[moduleActions[i]->getModule()->getIdentifier()].push_back(moduleActions[i]);

    std::map<std::string, std::vector<ModuleCallbackAction*> >::iterator mapIt;

    for (mapIt = callbackMapPerModule.begin(); mapIt!=callbackMapPerModule.end(); mapIt++) {
        std::vector<ModuleCallbackAction*> moduleActions = mapIt->second;

        if (moduleActions.size()) {
            QMenu* submenu = new QMenu(tr(mapIt->first.c_str())) ;
            moduleSubMenus_[mapIt->first.c_str()] = submenu;

            for (size_t i=0; i<moduleActions.size(); i++) {
                QAction* action = new QAction(tr(moduleActions[i]->getActionName().c_str()), this);
                action->setCheckable(true);
                submenu->addAction(action);
                action->setChecked(moduleActions[i]->getActionState() == ModuleCallBackActionState::Enabled);
                connect(action,SIGNAL(triggered()),this, SLOT(moduleAction()));
            }
        }
    }
}

void PropertyWidgetQt::updateModuleMenuActions() {
    InviwoApplication* app = InviwoApplication::getPtr();
    std::vector<ModuleCallbackAction*> moduleActions = app->getCallbackActions();

    for (size_t i=0; i<moduleActions.size(); i++) {
        std::string moduleName = moduleActions[i]->getModule()->getIdentifier();
        QMapIterator<QString,QMenu*> it(moduleSubMenus_);

        while (it.hasNext()) {
            it.next();

            if (it.key().toLocal8Bit().constData() == moduleName) {
                QList<QAction*> actions = it.value()->actions();

                for (int j=0; j<actions.size(); j++) {
                    if (actions[j]->text().toLocal8Bit().constData() == moduleActions[i]->getActionName()) {
                        //FIXME: Following setChecked is not behaving as expected on some special case. This needs to be investigated.
                        //bool blockState = actions[j]->blockSignals(true);
                        actions[j]->setChecked(moduleActions[i]->getActionState() == ModuleCallBackActionState::Enabled);
                        //actions[j]->blockSignals(blockState);
                    }
                }
            }
        }
    }
}

void PropertyWidgetQt::setDeveloperViewMode(bool value) {
    property_->setVisibility(DEVELOPMENT);
    developerViewModeAction_->setChecked(true);
    updateContextMenu();
}

void PropertyWidgetQt::setApplicationViewMode(bool value) {
    property_->setVisibility(APPLICATION);
    applicationViewModeAction_->setChecked(true);
    updateContextMenu();
}

PropertyVisibilityMode PropertyWidgetQt::getApplicationViewMode() {
    Settings* mainSettings = InviwoApplication::getPtr()->getSettingsByType<SystemSettings>();
    PropertyVisibilityMode appMode =  static_cast<PropertyVisibilityMode>(dynamic_cast<OptionPropertyInt*>
                                      (mainSettings->getPropertyByIdentifier("viewMode"))->get());
    return appMode;
}

void PropertyWidgetQt::moduleAction() {
    QAction* action = qobject_cast<QAction*>(QObject::sender());

    if (action) {
        InviwoApplication* app = InviwoApplication::getPtr();
        std::vector<ModuleCallbackAction*> moduleActions = app->getCallbackActions();
        std::string actionName(action->text().toLocal8Bit().constData()) ;

        for (size_t i=0; i<moduleActions.size(); i++) {
            if (moduleActions[i]->getActionName() == actionName) {
                moduleActions[i]->getCallBack()->invoke(property_);
                action->setChecked(moduleActions[i]->getActionState() == ModuleCallBackActionState::Enabled);
            }
        }
    }
}

void PropertyWidgetQt::updateContextMenu() {
    if (property_->getVisibilityMode() == DEVELOPMENT)
        developerViewModeAction_->setChecked(true);
    else if (property_->getVisibilityMode() == APPLICATION)
        applicationViewModeAction_->setChecked(true);

    updateModuleMenuActions();
}

void PropertyWidgetQt::setProperty(Property* property) {
    PropertyWidget::setProperty(property);
    setToolTip(("id: " + property_->getIdentifier()).c_str());
}

//////////////////////////////////////////////////////////////////////////

PropertyEditorWidgetQt::PropertyEditorWidgetQt(std::string widgetName, QWidget* parent)
    : InviwoDockWidget(QString(widgetName.c_str()), parent)
    , VoidObserver()
    , PropertyEditorWidget()
{
}

PropertyEditorWidgetQt::~PropertyEditorWidgetQt() { }

void PropertyEditorWidgetQt::initialize(Property* property) {
    PropertyEditorWidget::initialize(property);
}

void PropertyEditorWidgetQt::deinitialize() {
    PropertyEditorWidget::deinitialize();
    hide();
}



} // namespace
