#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/core/study/studyparameterlist.h>

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
    
    addToStudyAction_= new QAction(tr("&Add to Study"),this);
    addToStudyAction_->setCheckable(true);
    contextMenu_->addAction(addToStudyAction_);


    updateContextMenu();
    connect(this,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint&)));
    connect(developerViewModeAction_,SIGNAL(triggered(bool)),this, SLOT(setDeveloperViewMode(bool)));
    connect(applicationViewModeAction_,SIGNAL(triggered(bool)),this, SLOT(setApplicationViewMode(bool)));
    connect(addToStudyAction_,SIGNAL(triggered(bool)),this, SLOT(addToStudy(bool)));
}

void PropertyWidgetQt::showContextMenu(const QPoint& pos) {
    PropertyVisibilityMode appVisibilityMode  = getApplicationViewMode();
    if (appVisibilityMode == DEVELOPMENT) {
        updateContextMenu();
        QPoint globalPos = this->mapToGlobal(pos);
        QAction* selecteditem = contextMenu_->exec(globalPos);
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

    addToStudyAction_ = new QAction(tr("&Add to Study"),this);
    addToStudyAction_->setCheckable(true);
    contextMenu->addAction(addToStudyAction_);
   
    return contextMenu;
    
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

void PropertyWidgetQt::addToStudy(bool value) { 
    if ( !StudyParameterList::getPtr()->isParameterAdded(property_) ) {
        addToStudyAction_->setChecked(true);
        StudyParameterList::getPtr()->addParameter(InviwoApplication::getPtr()->getProcessorNetwork(), property_);
    }
    else
        StudyParameterList::getPtr()->removeParameter(property_);
}

void PropertyWidgetQt::setProperty(Property* prop) {
    property_ = prop;
}

void PropertyWidgetQt::updateContextMenu(){
    if (property_->getVisibilityMode() == DEVELOPMENT)
        developerViewModeAction_->setChecked(true);
    else if (property_->getVisibilityMode() == APPLICATION)
        applicationViewModeAction_->setChecked(true);

    //FIXME Should not be here?, or at least cause crash on  startup
    /*if ( StudyParameterList::getPtr()->isParameterAdded(property_) )
        addToStudyAction_->setChecked(true);  
    else {
        if (addToStudyAction_->isChecked())
             addToStudyAction_->setChecked(false);  
    }*/
}


} // namespace
