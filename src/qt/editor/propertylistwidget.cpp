#include <inviwo/qt/editor/propertylistwidget.h>
#include <inviwo/core/properties/propertywidgetfactory.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <QSettings>


namespace inviwo {

PropertyListWidget* PropertyListWidget::propertyListWidget_ = 0;

PropertyListWidget::PropertyListWidget(QWidget* parent) : InviwoDockWidget(tr("Properties"), parent), VoidObservable() {
    setObjectName("ProcessorListWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    propertyListWidget_ = this;
    
    scrollArea_ = new QScrollArea(propertyListWidget_);
    scrollArea_->setWidgetResizable(true);
    scrollArea_->setMinimumWidth(300);
    scrollArea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    listWidget_ = new QWidget();
    listWidgetLayout_ = new QVBoxLayout(listWidget_);
    listWidgetLayout_->setAlignment(Qt::AlignTop);

    scrollArea_->setWidget(listWidget_);
    setWidget(scrollArea_);

    checkBoxVisibility_ = NULL;
    QSettings settings("Inviwo", "Inviwo");
    settings.beginGroup("PropertyListwidget");
    checkBoxState_ =static_cast<Qt::CheckState>(settings.value("visibilityCheckbox",false).toInt());
    developerViewMode_ = settings.value("developerViewMode",true).toBool();
    applicationViewMode_ = settings.value("applicationViewMode",false).toBool();
    settings.endGroup();


}

PropertyListWidget::~PropertyListWidget() {}

void PropertyListWidget::showProcessorProperties(std::vector<Processor*> processors) {
    for (size_t i=0; i<processors.size(); i++) {
        addProcessorPropertiesToLayout(processors[i]);
    }
}

void PropertyListWidget::showProcessorProperties(Processor* processor) {
    // check if processor widget has been already generated
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(processor->getIdentifier());
    QWidget* processorPropertyWidget = 0;
    if (it != propertyWidgetMap_.end()) {
        // property widget has already been created and stored in the map
        processorPropertyWidget = it->second;
    } else {
        processorPropertyWidget = createNewProcessorPropertiesItem(processor);
    }

    if (processorPropertyWidget) {
        removeAllProcessorProperties();
        processorPropertyWidget->setVisible(true);
        listWidgetLayout_->addWidget(processorPropertyWidget);
    }
}

void PropertyListWidget::addProcessorPropertiesToLayout(Processor* processor) {
    // check if processor widget has been already generated
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(processor->getIdentifier());
    QWidget* processorPropertyWidget = 0;
    if (it != propertyWidgetMap_.end()) {
        // property widget has already been created and stored in the map
        processorPropertyWidget = it->second;
    } else {
        processorPropertyWidget = createNewProcessorPropertiesItem(processor); 
    }

    if (processorPropertyWidget) {
        processorPropertyWidget->setVisible(true);
        listWidgetLayout_->addWidget(processorPropertyWidget);
    }
}

void PropertyListWidget::removeAllProcessorProperties() {
    while (QLayoutItem* item = listWidgetLayout_->takeAt(0)) {
        QWidget* processorPropertyWidget = item->widget();
        listWidgetLayout_->removeWidget(processorPropertyWidget);
        processorPropertyWidget->setVisible(false);
    }
}

QWidget* PropertyListWidget::createNewProcessorPropertiesItem(Processor* processor) {
    // create property widget and store it in the map
    QWidget* processorPropertyWidget = new QWidget(this, Qt::WindowStaysOnBottomHint);
    QVBoxLayout* vLayout = new QVBoxLayout(processorPropertyWidget);
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setSpacing(0);
    vLayout->setMargin(0);
    vLayout->setContentsMargins(0,0,0,0);


    QLabel* processorLabel = new QLabel(QString::fromStdString(processor->getIdentifier()));
    processorLabel->setAlignment(Qt::AlignCenter);
    processorLabel->setAutoFillBackground(true);
    processorLabel->setFrameStyle(QFrame::StyledPanel);

    checkBoxVisibility_ = new QCheckBox();
    vLayout->addWidget(checkBoxVisibility_);
    checkBoxVisibility_->setCheckState(checkBoxState_);
    connect(checkBoxVisibility_,SIGNAL(stateChanged(int)),this,SLOT(checkBoxStateChange(int)));

    vLayout->addWidget(processorLabel);
    std::vector<Property*> properties = processor->getProperties();
    std::vector<Property*> addedProperties;
    for (size_t i=0; i<properties.size(); i++) {
        Property* curProperty = properties[i];
        // check if the property is already added
        if(std::find(addedProperties.begin(),addedProperties.end(),curProperty) != addedProperties.end())
            continue;
        // add to group box if one is assigned to the property
        else if (curProperty->getGroupID()!="") {
            CollapsiveGroupBoxWidgetQt* group = new CollapsiveGroupBoxWidgetQt(curProperty->getGroupID(),curProperty->getGroupDisplayName());
            // add all the properties with the same group assigned
            for (size_t k=0; k<properties.size(); k++){
                Property* tmpProperty = properties[k];
                if (curProperty->getGroupID() == tmpProperty->getGroupID()) {
                    group->addProperty(tmpProperty);
                    addedProperties.push_back(tmpProperty);
                }
            }
            group->generatePropertyWidgets();

            QHBoxLayout* hLayout = new QHBoxLayout();
            hLayout->setSpacing(0);
            hLayout->setMargin(0);
            hLayout->setContentsMargins(0,0,0,0);

            OptionPropertyInt* displayModeOption = new OptionPropertyInt("viewMode","",1,PropertyOwner::INVALID_OUTPUT,PropertySemantics::Custom);
            displayModeOption->addOption("Development","Development",0);
            displayModeOption->addOption("Application","Application",1);
            PropertyWidgetQt* viewModeWidget = PropertyWidgetFactoryQt::getRef().create(displayModeOption);

            if (curProperty->getVisible()!=PropertyVisibility::INVISIBLE)
                displayModeOption->setSelectedOption(curProperty->getVisible());

            viewModeWidget->updateFromProperty();

            hLayout->addWidget(viewModeWidget);
            hLayout->addWidget(group);
            group->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            vLayout->addLayout(hLayout,0);

            std::vector<PropertyWidgetQt*> widgets = group->getPropertyWidgets();
            for (size_t i=0; i<widgets.size(); i++) {
                connect(viewModeWidget,SIGNAL(optionChanged(int)),widgets[i],SLOT(visibilityModified(int)));
            }
            viewModeWidgets_.push_back(viewModeWidget);
        }
        else {
            PropertyWidgetQt* propertyWidget = PropertyWidgetFactoryQt::getRef().create(curProperty);
            if (propertyWidget) {
                QHBoxLayout* hLayout = new QHBoxLayout();
                hLayout->setSpacing(0);
                hLayout->setMargin(0);
                hLayout->setContentsMargins(0,0,0,0);

                OptionPropertyInt* displayModeOption = new OptionPropertyInt("viewMode","",1,PropertyOwner::INVALID_OUTPUT,PropertySemantics::Custom);
                displayModeOption->addOption("Development","Development",0);
                displayModeOption->addOption("Application","Application",1);
                PropertyWidgetQt* viewModeWidget = PropertyWidgetFactoryQt::getRef().create(displayModeOption);
                hLayout->addWidget(viewModeWidget);
                hLayout->addWidget(propertyWidget);
                vLayout->addLayout(hLayout,0);

                propertyWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
                curProperty->registerPropertyWidget(propertyWidget);

                if (curProperty->getVisible()!=PropertyVisibility::INVISIBLE)
                    displayModeOption->setSelectedOption(curProperty->getVisible());

                viewModeWidget->updateFromProperty();
                viewModeWidgets_.push_back(viewModeWidget);

                connect(viewModeWidget,SIGNAL(optionChanged(int)),propertyWidget,SLOT(visibilityModified(int)));
                connect(propertyWidget, SIGNAL(modified()), this, SLOT(propertyModified()));
                addedProperties.push_back(curProperty);
            }
        }
    } 
    vLayout->addStretch(1);
    propertyWidgetMap_.insert(std::make_pair(processor->getIdentifier(), processorPropertyWidget));
    checkBoxStateChange(checkBoxState_);
    setApplicationViewMode(applicationViewMode_);
    setDeveloperViewMode(developerViewMode_);

    return processorPropertyWidget;
}

void PropertyListWidget::removeProcessorProperties(Processor* processor) {
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(processor->getIdentifier());
    if (it != propertyWidgetMap_.end()) { 
        QWidget* processorPropertyWidget = propertyWidgetMap_[processor->getIdentifier()];
        listWidgetLayout_->removeWidget(processorPropertyWidget);
        processorPropertyWidget->setVisible(false);
        propertyWidgetMap_.erase(processor->getIdentifier());
        delete processorPropertyWidget;
    }
}

void PropertyListWidget::propertyModified() {
    notifyObservers();
}

PropertyListWidget* PropertyListWidget::instance() {
    return propertyListWidget_;
}

void PropertyListWidget::setDeveloperViewMode( bool value  ){
    if (value) {
        std::cout << "DeveloperMode" << std::endl;
        InviwoApplication* inviwoApp = InviwoApplication::getPtr();
        static_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode"))->set(0);
        
        if (checkBoxVisibility_!=NULL) {
            checkBoxVisibility_->show();
            checkBoxStateChange(checkBoxVisibility_->checkState());
        }
        developerViewMode_ = value;
        applicationViewMode_ = !value;
    }
}

void PropertyListWidget::setApplicationViewMode( bool value ){
    if (value) { 
        std::cout << "ApplicationMode" << std::endl;
        InviwoApplication* inviwoApp = InviwoApplication::getPtr();
        dynamic_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode"))->set(1);
        
        if (checkBoxVisibility_!=NULL) {
            checkBoxVisibility_->hide();
            hideViewModeWidgets();
        }

        applicationViewMode_ = value;
        developerViewMode_ = !value;
    }
}


void PropertyListWidget::checkBoxStateChange( int state ){
    if (state==0)
        showViewModeWidgets();
    else
        hideViewModeWidgets();

    checkBoxState_ =static_cast<Qt::CheckState>(state);
}

void PropertyListWidget::showViewModeWidgets(){
    for (size_t i=0; i<viewModeWidgets_.size(); i++)
        viewModeWidgets_[i]->showWidget();
}

void PropertyListWidget::hideViewModeWidgets(){
    for (size_t i=0; i<viewModeWidgets_.size(); i++)
        viewModeWidgets_[i]->hideWidget();
}


void PropertyListWidget::saveState(){
    QSettings settings("Inviwo", "Inviwo");
    settings.beginGroup("PropertyListwidget");
    settings.setValue("visibilityCheckbox",checkBoxState_);
    settings.setValue("developerViewMode", developerViewMode_);
    settings.setValue("applicationViewMode",applicationViewMode_);
    settings.endGroup();
}

PropertyVisibility::VisibilityMode PropertyListWidget::getVisibilityMode(){
    if (developerViewMode_) 
        return PropertyVisibility::DEVELOPMENT;
    else
        return PropertyVisibility::APPLICATION;
}

} // namespace