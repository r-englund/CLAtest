#include <inviwo/qt/widgets/properties/collapsivegroupboxwidgetqt.h>
#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>

namespace inviwo {

    CollapsiveGroupBoxWidgetQt::CollapsiveGroupBoxWidgetQt(std::string identifier, std::string displayName):
    identifier_(identifier),
    displayName_(displayName){
	collapsed_ = false;
    generateWidget();
    updateFromProperty();
}

void CollapsiveGroupBoxWidgetQt::generateWidget() {

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
    addObservation(tmpProperty);
    tmpProperty->addObserver(this);
}

void CollapsiveGroupBoxWidgetQt::generatePropertyWidgets() {
    PropertyWidgetFactoryQt* propertyWidgetFactory = new PropertyWidgetFactoryQt();
    for (size_t i=0; i<properties_.size(); i++) {
        Property* curProperty = properties_[i];
        PropertyWidgetQt* propertyWidget = propertyWidgetFactory->create(curProperty);
        vLayout_->addWidget(propertyWidget);
        curProperty->registerPropertyWidget(propertyWidget);
        connect(propertyWidget, SIGNAL(modified()), this, SLOT(propertyModified()));
    }
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

void CollapsiveGroupBoxWidgetQt::notify(){
    if (!this->isHidden()) {
        this->setVisible(false);
    }
    else{
        this->setVisible(true);
    }
}

void CollapsiveGroupBoxWidgetQt::propertyModified(){
    emit modified();
}

void CollapsiveGroupBoxWidgetQt::setGroupDisplayName(){
    displayName_ = label_->getText();
}

} // namespace
