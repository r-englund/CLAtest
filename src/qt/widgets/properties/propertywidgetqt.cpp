#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

namespace inviwo {

    PropertyWidgetQt::PropertyWidgetQt() {
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
        QMenu* viewModeItem_ = new QMenu(tr("&View mode"));

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
        updateContextMenu();
        connect(this,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint&)));
        connect(developerViewModeAction_,SIGNAL(triggered(bool)),this, SLOT(setDeveloperViewMode(bool)));
        connect(applicationViewModeAction_,SIGNAL(triggered(bool)),this, SLOT(setApplicationViewMode(bool)));
    }

    void PropertyWidgetQt::showContextMenu(const QPoint& pos) {
        InviwoApplication* inviwoApp = InviwoApplication::getPtr();
        PropertyVisibility::VisibilityMode appVisibilityMode  = static_cast<PropertyVisibility::VisibilityMode>(static_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode"))->get());
        if (appVisibilityMode == PropertyVisibility::DEVELOPMENT) {
            QPoint globalPos = this->mapToGlobal(pos);
            QAction* selecteditem = contextMenu_->exec(globalPos);
        }
    }
    QMenu* PropertyWidgetQt::generatePropertyWidgetMenu(){
        QMenu* contextMenu = new QMenu();
        viewModeItem_ = new QMenu(tr("&View mode"),contextMenu);

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
        return contextMenu;
    }

    void PropertyWidgetQt::setDeveloperViewMode(bool value) {
        property_->setVisibility(PropertyVisibility::DEVELOPMENT);
        developerViewModeAction_->setChecked(true);
    }

    void PropertyWidgetQt::setApplicationViewMode(bool value) {
        property_->setVisibility(PropertyVisibility::APPLICATION);
        applicationViewModeAction_->setChecked(true);
    }

    void PropertyWidgetQt::setProperty(Property* prop) {
        property_ = prop;
    }

    void PropertyWidgetQt::updateContextMenu(){
        if (property_->getVisibilityMode() == PropertyVisibility::DEVELOPMENT)
            developerViewModeAction_->setChecked(true);
        else if (property_->getVisibilityMode() == PropertyVisibility::APPLICATION)
            applicationViewModeAction_->setChecked(true);
    }
    //void PropertyWidgetQt::notify(){
    //    //InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    //    //int mode = static_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode"))->get();
    //    //if (mode == 0)
    //    //    viewModeItem_->setVisible(true);
    //    //if (mode == 1)
    //    //    viewModeItem_->setVisible(false);

    //}

} // namespace
