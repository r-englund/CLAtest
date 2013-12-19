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

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/core/study/studyparameterlist.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/core/common/moduleaction.h>

namespace inviwo {

PropertyWidgetQt::PropertyWidgetQt() : viewModeItem_(NULL) {
}

void PropertyWidgetQt::showWidget() {
    this->show();
    emit visibilityChange();
}
void PropertyWidgetQt::hideWidget() {
    this->hide();
    emit visibilityChange();
}

void PropertyWidgetQt::visibilityModified(int mode) {}

void PropertyWidgetQt::generateContextMenu(){
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    bool appInitialized = inviwoApp->isInitialized();
    //FIXME: Why do we need to check if inviwo initialized? Should addObservation exist here?
    ivwAssert(appInitialized!=false, "InviwoApplication not initialized.This should not be the case.");
    this->addObservation(static_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode")));
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    contextMenu_ = new QMenu();
    viewModeItem_ = new QMenu(tr("&View mode"));

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
QMenu* PropertyWidgetQt::generatePropertyWidgetMenu(){
    QMenu* contextMenu = new QMenu();
    if (viewModeItem_ != NULL) 
        contextMenu->addMenu(viewModeItem_);
    else{
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
    for (size_t i=0; i<moduleActions.size(); i++) {
        callbackMapPerModule[moduleActions[i]->getModule()->getIdentifier()].push_back(moduleActions[i]);
    }
    std::map<std::string, std::vector<ModuleCallbackAction*> >::iterator mapIt;
    for (mapIt = callbackMapPerModule.begin(); mapIt!=callbackMapPerModule.end(); mapIt++) {
        std::vector<ModuleCallbackAction*> moduleActions = mapIt->second;
        if (moduleActions.size()) {
            QMenu* submenu = new QMenu( tr(mapIt->first.c_str()) ) ;
            moduleSubMenus_[mapIt->first.c_str()] = submenu;
            for (size_t i=0; i<moduleActions.size(); i++) {
                QAction* action = new QAction( tr(moduleActions[i]->getActionName().c_str()), this);
                actionS_.push_back(action);
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
                
                QList<QAction*> actions1 = actionS_;
                QList<QAction*> actions = it.value()->actions();
                for (int j=0; j<actions.size(); j++) {
                     
                    if (actions[j]->text().toLocal8Bit().constData() == moduleActions[i]->getActionName()){   
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

PropertyVisibilityMode PropertyWidgetQt::getApplicationViewMode(){
    return InviwoApplication::getPtr()->getPropertyVisibilityMode();
}

void PropertyWidgetQt::moduleAction() { 
    QAction* action = qobject_cast<QAction*>(QObject::sender());
    if(action) {
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

void PropertyWidgetQt::setProperty(Property* prop) {
    property_ = prop;
}

void PropertyWidgetQt::updateContextMenu(){
    if (property_->getVisibilityMode() == DEVELOPMENT)
        developerViewModeAction_->setChecked(true);
    else if (property_->getVisibilityMode() == APPLICATION)
        applicationViewModeAction_->setChecked(true);
    
    updateModuleMenuActions();
}


} // namespace
