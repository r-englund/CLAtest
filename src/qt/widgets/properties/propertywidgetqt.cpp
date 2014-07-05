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
#include <inviwo/core/properties/propertywidgetfactory.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/core/common/moduleaction.h>
#include <QDesktopWidget>

#include <QStyleOption>
#include <QPainter>
#include <QToolTip>
#include <QHelpEvent>

namespace inviwo {

int PropertyWidgetQt::MINIMUM_WIDTH = 250;
int PropertyWidgetQt::SPACING = 7;
int PropertyWidgetQt::MARGIN = 0;

PropertyWidgetQt::PropertyWidgetQt()
    : QWidget()
    , PropertyWidget()
    , usageModeItem_(NULL)
    , usageModeActionGroup_(NULL)
    , developerUsageModeAction_(NULL)
    , applicationUsageModeAction_(NULL)
    , semanicsMenuItem_(NULL)
    , semanticsActionGroup_(NULL)
    , contextMenu_(NULL)
    , cached_(true) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this,
            SLOT(showContextMenu(const QPoint&)));
}

PropertyWidgetQt::PropertyWidgetQt(Property* property)
    : QWidget()
    , PropertyWidget(property)
    , usageModeItem_(NULL)
    , usageModeActionGroup_(NULL)
    , developerUsageModeAction_(NULL)
    , applicationUsageModeAction_(NULL)
    , semanicsMenuItem_(NULL)
    , semanticsActionGroup_(NULL)
    , contextMenu_(NULL)
    , cached_(true) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this,
            SLOT(showContextMenu(const QPoint&)));
}

PropertyWidgetQt::~PropertyWidgetQt() {}

void PropertyWidgetQt::setVisible(bool visible) {
    if (cached_) return;

    if (visible) {
        QWidget::setVisible(true);
    } else {
        QWidget::setVisible(false);
    }
}

void PropertyWidgetQt::showWidget() {
    cached_ = false;
    if (isHidden() && getVisible()) {
        updateContextMenu();
        this->show();
    }
}

void PropertyWidgetQt::hideWidget() {
    if (isVisible()) {
        this->hide();
    }
}

void PropertyWidgetQt::initializeEditorWidgetsMetaData() {
    if (hasEditorWidget()) {
        // Validates editor widget position
        PropertyEditorWidgetQt* propertyEditorWidget =
            dynamic_cast<PropertyEditorWidgetQt*>(getEditorWidget());
        InviwoApplicationQt* app = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());
        if (!propertyEditorWidget) return;

        // set widget meta data stuff

        PropertyEditorWidgetDockStatus docStatus = propertyEditorWidget->getEditorDockStatus();

        if (docStatus == PropertyEditorWidgetDockStatus::DockedLeft) {
            app->getMainWindow()->addDockWidget(Qt::LeftDockWidgetArea, propertyEditorWidget);
            propertyEditorWidget->setFloating(false);
        } else if (docStatus == PropertyEditorWidgetDockStatus::DockedRight) {
            app->getMainWindow()->addDockWidget(Qt::RightDockWidgetArea, propertyEditorWidget);
            propertyEditorWidget->setFloating(false);
        } else {
            app->getMainWindow()->addDockWidget(Qt::RightDockWidgetArea, propertyEditorWidget);
            propertyEditorWidget->setFloating(true);
        }

        propertyEditorWidget->hide();

        ivec2 widgetDimension = getEditorWidget()->getEditorDimensionMetaData();
        propertyEditorWidget->resize(QSize(widgetDimension.x, widgetDimension.y));

        // TODO: Desktop screen info should be added to system capabilities
        ivec2 pos = getEditorWidget()->getEditorPositionMetaData();
        QDesktopWidget* desktop = QApplication::desktop();
        int primaryScreenIndex = desktop->primaryScreen();
        QRect wholeScreenGeometry = desktop->screenGeometry(primaryScreenIndex);
        QRect primaryScreenGeometry = desktop->screenGeometry(primaryScreenIndex);

        for (int i = 0; i < desktop->screenCount(); i++) {
            if (i != primaryScreenIndex)
                wholeScreenGeometry = wholeScreenGeometry.united(desktop->screenGeometry(i));
        }

        wholeScreenGeometry.setRect(wholeScreenGeometry.x() - 10, wholeScreenGeometry.y() - 10,
                                    wholeScreenGeometry.width() + 20,
                                    wholeScreenGeometry.height() + 20);
        QPoint bottomRight = QPoint(pos.x + this->width(), pos.y + this->height());

        QPoint appPos = app->getMainWindow()->pos();

        if (!wholeScreenGeometry.contains(QPoint(pos.x, pos.y)) ||
            !wholeScreenGeometry.contains(bottomRight)) {
            pos = ivec2(appPos.x(), appPos.y());
            pos += ivec2(primaryScreenGeometry.width() / 2, primaryScreenGeometry.height() / 2);
            propertyEditorWidget->move(pos.x, pos.y);
        } else {
            if (!(pos.x == 0 && pos.y == 0))
                propertyEditorWidget->move(pos.x, pos.y);
            else {
                pos = ivec2(appPos.x(), appPos.y());
                pos += ivec2(primaryScreenGeometry.width() / 2, primaryScreenGeometry.height() / 2);
                propertyEditorWidget->move(pos.x, pos.y);
            }
        }

        bool visible = getEditorWidget()->getEditorVisibilityMetaData();
        if (!visible)
            propertyEditorWidget->hide();
        else
            propertyEditorWidget->show();
    }
}

void PropertyWidgetQt::paintEvent(QPaintEvent* pe) {
    QStyleOption o;
    o.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
};

//void PropertyWidgetQt::visibilityModified(int mode) {}

void PropertyWidgetQt::generateContextMenu() {
    if (!contextMenu_) {
        contextMenu_ = new QMenu(this);

        // View mode actions (Developer / Application)
        usageModeItem_ = new QMenu(tr("&Usage mode"), contextMenu_);
        developerUsageModeAction_ = new QAction(tr("&Developer"), this);
        developerUsageModeAction_->setCheckable(true);
        usageModeItem_->addAction(developerUsageModeAction_);

        applicationUsageModeAction_ = new QAction(tr("&Application"), this);
        applicationUsageModeAction_->setCheckable(true);
        usageModeItem_->addAction(applicationUsageModeAction_);

        usageModeActionGroup_ = new QActionGroup(this);
        usageModeActionGroup_->addAction(developerUsageModeAction_);
        usageModeActionGroup_->addAction(applicationUsageModeAction_);
        contextMenu_->addMenu(usageModeItem_);

        if (property_) {
            semanicsMenuItem_ = new QMenu(tr("&Semantics"), contextMenu_);
            semanticsActionGroup_ = new QActionGroup(this);
            
            std::vector<PropertySemantics> semantics =
                PropertyWidgetFactory::getPtr()->getSupportedSemanicsForProperty(property_);
        
            
            for (std::vector<PropertySemantics>::iterator it = semantics.begin();
                 it!=semantics.end(); ++it) {
            
                QAction* semanticAction = new QAction(QString::fromStdString(it->getString()),semanticsActionGroup_);
                semanticAction->setCheckable(true);
                semanicsMenuItem_->addAction(semanticAction);
                if (*it == property_->getSemantics()) {
                    semanticAction->setChecked(true);
                } else {
                    semanticAction->setChecked(false);
                }
                semanticAction->setData(QString::fromStdString(it->getString()));
            }
            
            connect(semanticsActionGroup_, SIGNAL(triggered(QAction*)),
                    this, SLOT(changeSemantics(QAction*)));
        
            
            contextMenu_->addMenu(semanicsMenuItem_);
        }
        
        QAction* resetAction = new QAction(tr("&Reset to default"), this);
        resetAction->setToolTip(tr("&Reset the property back to it's initial state"));
        contextMenu_->addAction(resetAction);

        connect(developerUsageModeAction_, SIGNAL(triggered(bool)), this,
                SLOT(setDeveloperUsageMode(bool)));

        connect(applicationUsageModeAction_, SIGNAL(triggered(bool)), this,
                SLOT(setApplicationUsageMode(bool)));

        connect(resetAction, SIGNAL(triggered()), this, SLOT(resetPropertyToDefaultState()));

        // Module actions.
        generateModuleMenuActions();
        QMapIterator<QString, QMenu*> it(moduleSubMenus_);

        while (it.hasNext()) {
            it.next();
            contextMenu_->addMenu(it.value());
        }

        // Update to current state.
        updateContextMenu();
    }
}

void PropertyWidgetQt::changeSemantics(QAction* action) {
    PropertySemantics semantics(action->data().toString().toStdString());
    LogInfo("Change semanics to: " << semantics.getString());
    if (property_) {
        property_->setSemantics(semantics);
        emit updateSemantics(this);
    }
}
    
void PropertyWidgetQt::showContextMenu(const QPoint& pos) {
    if (!contextMenu_) {
        generateContextMenu();
    }
    updateContextMenu();
    contextMenu_->exec(this->mapToGlobal(pos));
}

QMenu* PropertyWidgetQt::getContextMenu() {
    if (!contextMenu_) {
        generateContextMenu();
    }
    return contextMenu_;
}

void PropertyWidgetQt::generateModuleMenuActions() {
    moduleSubMenus_.clear();
    InviwoApplication* app = InviwoApplication::getPtr();
    std::vector<ModuleCallbackAction*> moduleActions = app->getCallbackActions();
    std::map<std::string, std::vector<ModuleCallbackAction*> > callbackMapPerModule;

    for (size_t i = 0; i < moduleActions.size(); i++)
        callbackMapPerModule[moduleActions[i]->getModule()->getIdentifier()].push_back(
            moduleActions[i]);

    std::map<std::string, std::vector<ModuleCallbackAction*> >::iterator mapIt;

    for (mapIt = callbackMapPerModule.begin(); mapIt != callbackMapPerModule.end(); mapIt++) {
        std::vector<ModuleCallbackAction*> moduleActions = mapIt->second;

        if (moduleActions.size()) {
            QMenu* submenu = new QMenu(tr(mapIt->first.c_str()));
            moduleSubMenus_[mapIt->first.c_str()] = submenu;

            for (size_t i = 0; i < moduleActions.size(); i++) {
                QAction* action = new QAction(tr(moduleActions[i]->getActionName().c_str()), this);
                action->setCheckable(true);
                submenu->addAction(action);
                action->setChecked(moduleActions[i]->getActionState() ==
                                   ModuleCallBackActionState::Enabled);
                connect(action, SIGNAL(triggered()), this, SLOT(moduleAction()));
            }
        }
    }
}

void PropertyWidgetQt::updateModuleMenuActions() {
    InviwoApplication* app = InviwoApplication::getPtr();
    std::vector<ModuleCallbackAction*> moduleActions = app->getCallbackActions();

    for (size_t i = 0; i < moduleActions.size(); i++) {
        std::string moduleName = moduleActions[i]->getModule()->getIdentifier();
        QMapIterator<QString, QMenu*> it(moduleSubMenus_);

        while (it.hasNext()) {
            it.next();

            if (it.key().toLocal8Bit().constData() == moduleName) {
                QList<QAction*> actions = it.value()->actions();

                for (int j = 0; j < actions.size(); j++) {
                    if (actions[j]->text().toLocal8Bit().constData() ==
                        moduleActions[i]->getActionName()) {
                        // FIXME: Following setChecked is not behaving as expected on some special
                        // case. This needs to be investigated.
                        // bool blockState = actions[j]->blockSignals(true);
                        actions[j]->setChecked(moduleActions[i]->getActionState() ==
                                               ModuleCallBackActionState::Enabled);
                        // actions[j]->blockSignals(blockState);
                    }
                }
            }
        }
    }
}

UsageMode PropertyWidgetQt::getUsageMode() const {
    return property_->getUsageMode();
};

bool PropertyWidgetQt::getVisible() const {
    return property_->getVisible();
}

void PropertyWidgetQt::setDeveloperUsageMode(bool value) {
    property_->setUsageMode(DEVELOPMENT);
    updateContextMenu();
    emit usageModeChanged();
}

void PropertyWidgetQt::setApplicationUsageMode(bool value) {
    property_->setUsageMode(APPLICATION);
    updateContextMenu();
    emit usageModeChanged();
}

UsageMode PropertyWidgetQt::getApplicationUsageMode() {
    return InviwoApplication::getPtr()->getSettingsByType<SystemSettings>()
                                      ->getApplicationUsageMode();
}

void PropertyWidgetQt::moduleAction() {
    QAction* action = qobject_cast<QAction*>(QObject::sender());

    if (action) {
        InviwoApplication* app = InviwoApplication::getPtr();
        std::vector<ModuleCallbackAction*> moduleActions = app->getCallbackActions();
        std::string actionName(action->text().toLocal8Bit().constData());

        for (size_t i = 0; i < moduleActions.size(); i++) {
            if (moduleActions[i]->getActionName() == actionName) {
                moduleActions[i]->getCallBack()->invoke(property_);
                action->setChecked(moduleActions[i]->getActionState() ==
                                   ModuleCallBackActionState::Enabled);
            }
        }
    }
}

void PropertyWidgetQt::updateContextMenu() {
    if (usageModeItem_) {
        // Update the current selection.
        if (getUsageMode() == DEVELOPMENT)
            developerUsageModeAction_->setChecked(true);
        else if (getUsageMode() == APPLICATION)
            applicationUsageModeAction_->setChecked(true);

        // Disable the view mode buttons in Application mode
        UsageMode appVisibilityMode = getApplicationUsageMode();
        if (appVisibilityMode == DEVELOPMENT) {
            developerUsageModeAction_->setEnabled(true);
            applicationUsageModeAction_->setEnabled(true);
        } else {
            developerUsageModeAction_->setEnabled(false);
            applicationUsageModeAction_->setEnabled(false);
        }
        updateModuleMenuActions();
    }
}

bool PropertyWidgetQt::event(QEvent* event) {
    if (event->type() == QEvent::ToolTip) {
        QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);
        QToolTip::showText(helpEvent->globalPos(), QString::fromStdString(getToolTipText()));
        return true;
    }
    return QWidget::event(event);
}

std::string PropertyWidgetQt::getToolTipText() {
    if (property_) {
        return "id: " + property_->getIdentifier();
    } else {
        return "";
    }
}

void PropertyWidgetQt::setProperty(Property* property) { PropertyWidget::setProperty(property); }

void PropertyWidgetQt::resetPropertyToDefaultState() { property_->resetToDefaultState(); }

void PropertyWidgetQt::setSpacingAndMargins(QLayout* layout) {
    layout->setContentsMargins(MARGIN, MARGIN, MARGIN, MARGIN);
    layout->setSpacing(SPACING);
}

QSize PropertyWidgetQt::sizeHint() const {
    QSize size = layout()->sizeHint();
    return size;
}

QSize PropertyWidgetQt::minimumSizeHint() const {
    QSize size = layout()->sizeHint();
    QSize minSize = layout()->minimumSize();
    size.setWidth(minSize.width());
    return size;
}

//////////////////////////////////////////////////////////////////////////

PropertyEditorWidgetQt::PropertyEditorWidgetQt(std::string widgetName, QWidget* parent)
    : InviwoDockWidget(QString(widgetName.c_str()), parent), PropertyEditorWidget() {}

PropertyEditorWidgetQt::~PropertyEditorWidgetQt() {}

void PropertyEditorWidgetQt::initialize(Property* property) {
    PropertyEditorWidget::initialize(property);
}

void PropertyEditorWidgetQt::deinitialize() {
    PropertyEditorWidget::deinitialize();
    hide();
}

}  // namespace
