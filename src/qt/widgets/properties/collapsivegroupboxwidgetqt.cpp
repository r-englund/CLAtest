#include <inviwo/qt/widgets/properties/collapsivegroupboxwidgetqt.h>
#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>

namespace inviwo {

CollapsiveGroupBoxWidgetQt::CollapsiveGroupBoxWidgetQt(std::string name) {
    name_=name;
	collapsed_ = false;
    generateWidget();
    updateFromProperty();
}

void CollapsiveGroupBoxWidgetQt::generateWidget() {

    btnCollapse_ = new QPushButton("-");
    btnCollapse_->setFixedSize(15,15);
    btnCollapse_->setStyleSheet("min-height: 10px; min-width: 10px;");

    QHBoxLayout* hLayout = new QHBoxLayout();
    QHBoxLayout* boxLayout = new QHBoxLayout();
    QGridLayout* gridLayout = new QGridLayout();
    
    groupBox_ = new QGroupBox();
    groupBox_->setFlat(true);

    QFrame* frame = new QFrame();
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    frame->setLineWidth(2);
    hLayout->addWidget(frame);

    vLayout_ = new QVBoxLayout();
    vLayout_->setAlignment(Qt::AlignTop);
    
    
    groupBox_->setLayout(vLayout_);
    groupBox_->layout()->setContentsMargins(0,0,0,0);
    groupBox_->setStyleSheet("border:0;");
    
    gridLayout->setContentsMargins(0,0,0,0);
    gridLayout->setSpacing(0);
    gridLayout->addWidget(btnCollapse_,1,1,1,1,Qt::AlignLeft);
    gridLayout->addWidget(new QLabel(QString::fromStdString(name_)),1,2,1,1,Qt::AlignLeft);
    gridLayout->addWidget(groupBox_,1,2,4,4,Qt::AlignTop);

    frame->setLayout(gridLayout);

    setLayout(hLayout);
   
    show();
}

void CollapsiveGroupBoxWidgetQt::updateFromProperty() {
    //for (size_t i=0; i<subPropertyWidgets_.size(); i++)
    //    subPropertyWidgets_[i]->updateFromProperty();
}

void CollapsiveGroupBoxWidgetQt::show() {
	collapsed_ = false;
    groupBox_->show();
    btnCollapse_->setText("-");
    connect(btnCollapse_,SIGNAL(clicked()),this,SLOT(hide()));
}

void CollapsiveGroupBoxWidgetQt::hide() {
	collapsed_= true;
    groupBox_->hide();
    btnCollapse_->setText("+");
    connect(btnCollapse_,SIGNAL(clicked()),this,SLOT(show()));
}

void CollapsiveGroupBoxWidgetQt::addProperty( Property* tmpProperty ) {
    properties_.push_back(tmpProperty);
}

void CollapsiveGroupBoxWidgetQt::generatePropertyWidgets() {
    vLayout_->addWidget(new QLabel(QString::fromStdString(name_)));
    PropertyWidgetFactoryQt* propertyWidgetFactory = new PropertyWidgetFactoryQt();
    for (size_t i=0; i<properties_.size(); i++) {
        Property* curProperty = properties_[i];
        PropertyWidgetQt* propertyWidget = propertyWidgetFactory->create(curProperty);
        vLayout_->addWidget(propertyWidget);
        curProperty->registerPropertyWidget(propertyWidget);
    }
}

void CollapsiveGroupBoxWidgetQt::generateEventPropertyWidgets(EventPropertyManager* eventPropertyManager) {
	vLayout_->addWidget(new QLabel(QString::fromStdString(name_)));
	PropertyWidgetFactoryQt* propertyWidgetFactory = new PropertyWidgetFactoryQt();
	for (size_t i=0; i<properties_.size(); i++) {
		Property* curProperty = properties_[i];
		PropertyWidgetQt* propertyWidget = propertyWidgetFactory->create(curProperty);
		vLayout_->addWidget(propertyWidget);
		curProperty->registerPropertyWidget(propertyWidget);
		dynamic_cast<EventPropertyWidgetQt*>(propertyWidget)->setManager(eventPropertyManager);
	}
}


std::string CollapsiveGroupBoxWidgetQt::getName(){
    return name_;
}

std::vector<Property*> CollapsiveGroupBoxWidgetQt::getProperties() {
    return properties_;
}

} // namespace
