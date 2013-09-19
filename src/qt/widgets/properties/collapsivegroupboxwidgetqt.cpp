#include <inviwo/qt/widgets/properties/collapsivegroupboxwidgetqt.h>

namespace inviwo {

CollapsiveGroupBoxWidgetQt::CollapsiveGroupBoxWidgetQt(std::string identifier, std::string displayName):
identifier_(identifier),
displayName_(displayName),
visibilityMode_(PropertyVisibility::APPLICATION){
    collapsed_ = false;
    generateWidget();
    updateFromProperty();
}

void CollapsiveGroupBoxWidgetQt::generateWidget() {

    setContextMenuPolicy(Qt::CustomContextMenu);
    btnCollapse_ = new QToolButton();
    btnCollapse_->setStyleSheet("QToolButton::hover { background-color: gray }");
    QHBoxLayout* hLayout = new QHBoxLayout();
    QHBoxLayout* boxLayout = new QHBoxLayout();
    QGridLayout* gridLayout = new QGridLayout();

    QHBoxLayout* H2 = new QHBoxLayout();

    groupBox_ = new QGroupBox();

    groupBox_->setFlat(true);
    groupBox_->setStyleSheet("border:0;");

    QFrame* frame = new QFrame();
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    frame->setLineWidth(2);
    hLayout->addWidget(frame);

    vLayout_ = new QVBoxLayout();
    vLayout_->setAlignment(Qt::AlignRight);
    vLayout_->setAlignment(Qt::AlignTop);
    vLayout_->setMargin(0);
    vLayout_->setSpacing(0);

    groupBox_->setLayout(vLayout_);

    gridLayout->setContentsMargins(0,0,0,0);
    gridLayout->setSpacing(0);
    H2->addWidget(btnCollapse_);
    label_ = new EditableLabelQt(displayName_);
    H2->addWidget(label_);
    H2->setSpacing(7);
    gridLayout->addLayout(H2,1,0,Qt::AlignLeft);
    gridLayout->addWidget(groupBox_,2,0);

    frame->setLayout(gridLayout);
    setLayout(hLayout);
    show();
}

void CollapsiveGroupBoxWidgetQt::updateFromProperty() {}

void CollapsiveGroupBoxWidgetQt::show() {
    collapsed_ = false;
    groupBox_->show();
    btnCollapse_->setIcon(QIcon(":/stylesheets/images/arrow_darker_down.png"));
    disconnect(btnCollapse_,SIGNAL(clicked()),this,SLOT(show()));
    connect(btnCollapse_,SIGNAL(clicked()),this,SLOT(hide()));
}

void CollapsiveGroupBoxWidgetQt::hide() {
    collapsed_= true;
    groupBox_->hide();
    btnCollapse_->setIcon(QIcon(":/stylesheets/images/arrow_darker_right.png"));
    disconnect(btnCollapse_,SIGNAL(clicked()),this,SLOT(hide()));
    connect(btnCollapse_,SIGNAL(clicked()),this,SLOT(show()));
}

void CollapsiveGroupBoxWidgetQt::addProperty( Property* tmpProperty ) {
    properties_.push_back(tmpProperty);
}

void CollapsiveGroupBoxWidgetQt::generatePropertyWidgets() {
    PropertyWidgetFactoryQt* propertyWidgetFactory = new PropertyWidgetFactoryQt();
    for (size_t i=0; i<properties_.size(); i++) {
        Property* curProperty = properties_[i];
        PropertyWidgetQt* propertyWidget = propertyWidgetFactory->create(curProperty);
        vLayout_->addWidget(propertyWidget);
        curProperty->registerPropertyWidget(propertyWidget);
        propertyWidgets_.push_back(propertyWidget);
        connect(propertyWidget, SIGNAL(modified()), this, SLOT(propertyModified()));
        connect(propertyWidget, SIGNAL(visibilityChange()),this, SLOT(updateVisibility()));
    }
    generateContextMenu();
}

void CollapsiveGroupBoxWidgetQt::generateEventPropertyWidgets(EventPropertyManager* eventPropertyManager) {
    PropertyWidgetFactoryQt* propertyWidgetFactory = new PropertyWidgetFactoryQt();
    for (size_t i=0; i<properties_.size(); i++) {
        Property* curProperty = properties_[i];
        PropertyWidgetQt* propertyWidget = propertyWidgetFactory->create(curProperty);
        vLayout_->addWidget(propertyWidget);
        curProperty->registerPropertyWidget(propertyWidget);
        dynamic_cast<EventPropertyWidgetQt*>(propertyWidget)->setManager(eventPropertyManager);
    }
}


std::string CollapsiveGroupBoxWidgetQt::getIdentifier() const {
    return identifier_;
}

void CollapsiveGroupBoxWidgetQt::setIdentifier(const std::string& identifier) {
    identifier_ = identifier;
}

std::string CollapsiveGroupBoxWidgetQt::getDisplayName() const {
    return displayName_;
}

void CollapsiveGroupBoxWidgetQt::setDisplayName(const std::string& displayName) {
    displayName_ = displayName;
}

std::vector<Property*> CollapsiveGroupBoxWidgetQt::getProperties() {
    return properties_;
}

void CollapsiveGroupBoxWidgetQt::propertyModified(){
    emit modified();
}

void CollapsiveGroupBoxWidgetQt::setGroupDisplayName(){
    displayName_ = label_->getText();
    Property::setGroupDisplayName(identifier_,displayName_);
}

void CollapsiveGroupBoxWidgetQt::updateVisibility(){

    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    PropertyVisibility::VisibilityMode visibilityMode  = static_cast<PropertyVisibility::VisibilityMode>(static_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode"))->get());

    if (visibilityMode == PropertyVisibility::DEVELOPMENT) {
        this->setContextMenuPolicy(Qt::CustomContextMenu);
        for (size_t i= 0; i<properties_.size();i++) {
            if(properties_[i]->getVisibilityMode()!= PropertyVisibility::INVISIBLE){
                this->setVisible(true);
                break;
            }
            this->setVisible(false);
        }
    }
    if (visibilityMode == PropertyVisibility::APPLICATION) {
        this->setContextMenuPolicy(Qt::NoContextMenu);
        for (size_t i= 0; i<properties_.size();i++) {
            if(properties_[i]->getVisibilityMode()== PropertyVisibility::APPLICATION ){
                this->setVisible(true);
                break;
            }
            this->setVisible(false);
        }

    }

}

void CollapsiveGroupBoxWidgetQt::generateContextMenu() {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
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

void CollapsiveGroupBoxWidgetQt::showContextMenu( const QPoint& pos ){
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    PropertyVisibility::VisibilityMode appVisibilityMode  = static_cast<PropertyVisibility::VisibilityMode>(static_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode"))->get());
    if (appVisibilityMode == PropertyVisibility::DEVELOPMENT) {
        QPoint globalPos = this->mapToGlobal(pos);
        QAction* selecteditem = contextMenu_->exec(globalPos);
    }
}


void CollapsiveGroupBoxWidgetQt::setDeveloperViewMode( bool value ){
    visibilityMode_ = PropertyVisibility::DEVELOPMENT;
    for (size_t i=0; i<properties_.size(); i++) {
        properties_.at(i)->setVisibility(PropertyVisibility::DEVELOPMENT);
    }
    developerViewModeAction_->setChecked(true);
    updateWidgets();
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    PropertyVisibility::VisibilityMode visibilityMode  = static_cast<PropertyVisibility::VisibilityMode>(static_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode"))->get());

    if (visibilityMode == PropertyVisibility::DEVELOPMENT)
        this->setVisible(true);
    if (visibilityMode == PropertyVisibility::APPLICATION)
        this->setVisible(false);

    emit visibilityModified();

}

void CollapsiveGroupBoxWidgetQt::setApplicationViewMode( bool value ){
    visibilityMode_ = PropertyVisibility::APPLICATION;
    for (size_t i=0; i<properties_.size(); i++) {
        properties_.at(i)->setVisibility(PropertyVisibility::APPLICATION);
    }
    applicationViewModeAction_->setChecked(true);
    updateWidgets();
    this->setVisible(true);

    emit visibilityModified();
}

void CollapsiveGroupBoxWidgetQt::updateContextMenu(){
    if (visibilityMode_ == PropertyVisibility::DEVELOPMENT)
        developerViewModeAction_->setChecked(true);
    if (visibilityMode_ == PropertyVisibility::APPLICATION)
        applicationViewModeAction_->setChecked(true);
}

void CollapsiveGroupBoxWidgetQt::updateWidgets() {
    for (size_t i=0; i<propertyWidgets_.size();i++) {
        propertyWidgets_.at(i)->updateContextMenu();
    }
}

} // namespace
