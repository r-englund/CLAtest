#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>

#include <inviwo/core/network/processornetwork.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/study/studyparameterlist.h>
#include <inviwo/qt/widgets/studyparameterlistwidget.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
#include <inviwo/qt/widgets/propertyselectiontreewidget.h>

namespace inviwo{

StudyParameterListWidget::StudyParameterListWidget(QWidget* parent) : QWidget(parent)
{
    addWorkspaceProperty_ = new FileProperty("externalWorkSpace", "Add External Workspace");
    setObjectName("StudyParameterListWidget");
    StudyParameterList::init();
    addObservation(StudyParameterList::getPtr());    
    addWorkspaceProperty_->onChange(this, &StudyParameterListWidget::addWorkspace);
    buildWidgets();
    updateStudyParameterList();
}

StudyParameterListWidget::~StudyParameterListWidget() {
    delete addWorkspaceProperty_;
}

void StudyParameterListWidget::buildWidgets() {   
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QVBoxLayout* vWorkspaceLayout = new QVBoxLayout();
    vWorkspaceLayout->setAlignment(Qt::AlignTop);

    PropertyWidgetQt* propertyWidget = 0;
    propertyWidget = PropertyWidgetFactoryQt::getRef().create(addWorkspaceProperty_);    
    addWorkspaceProperty_->registerPropertyWidget(propertyWidget);     
    vWorkspaceLayout->addWidget(propertyWidget);

    propertyList_ = new QListWidget(this);    
    connect(propertyList_, SIGNAL(itemDoubleClicked(QListWidgetItem *, int)), this, SLOT(onListItemDoubleClicked(QListWidgetItem *, int)));
    vWorkspaceLayout->addWidget(propertyList_);   

    QHBoxLayout* rightPushButtonLayout = new QHBoxLayout;    
    rightPushButtonLayout->setAlignment(Qt::AlignLeft);
    QPushButton* clearPushButton_ = new QPushButton("Clear");
    connect(clearPushButton_, SIGNAL(clicked()), this, SLOT(clickedClearButton()));    
    rightPushButtonLayout->addWidget(clearPushButton_);
    vWorkspaceLayout->addLayout(rightPushButtonLayout);

    mainLayout->addLayout(vWorkspaceLayout);        
}

void StudyParameterListWidget::notify() {
    updateStudyParameterList();
}

void StudyParameterListWidget::addWorkspace() {
    IvwDeserializer xmlDeserializer(addWorkspaceProperty_->get());
    ProcessorNetwork* processorNetwork = new ProcessorNetwork();
    processorNetwork->deserialize(xmlDeserializer);  
    std::vector<Property*> selectedProperties;
    PropertySelectionTreeDialog* propertySelectDialog = new PropertySelectionTreeDialog(processorNetwork, selectedProperties, this);
    if (propertySelectDialog->exec()==QDialog::Accepted) {
        LogWarn("Property Selection dialog closed: Count" << selectedProperties.size());
        for (size_t i=0; i<selectedProperties.size(); i++)
            StudyParameterList::getPtr()->addParameter(processorNetwork, selectedProperties[i]);
    }

}

void StudyParameterListWidget::updateStudyParameterList() {
    propertyList_->clear();
    std::map<ProcessorNetwork*, std::vector<Property*> >* parameterProperties = StudyParameterList::getPtr()->getParameters();
    std::map<ProcessorNetwork*, std::vector<Property*> >::iterator mapIt;
    for (mapIt = parameterProperties->begin(); mapIt!=parameterProperties->end(); mapIt++) {   
        std::vector<Property*> properties = mapIt->second;
        for (size_t i=0; i<properties.size(); i++)
            propertyList_->addItem(new QListWidgetItem( tr(properties[i]->getDisplayName().c_str()) ));
    }
}

void StudyParameterListWidget::clickedClearButton() {
    StudyParameterList::getPtr()->clearAllParameters();
    updateStudyParameterList();
}

void StudyParameterListWidget::onListItemDoubleClicked(QListWidgetItem * item, int column) {
    std::string info = item->text().toLocal8Bit().constData();
    info+=" :Property";
    LogWarn(info);
}


StudyParameterDialog::StudyParameterDialog(QWidget* parent) : QDialog(parent) {    
    initDialog();    
}

void StudyParameterDialog::initDialog() {
    std::string title = std::string("Property Selection");
    setWindowTitle(tr(title.c_str()));

    QSize rSize(384,756);
    setFixedSize(rSize);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    studyParameterListWidget_ = new StudyParameterListWidget(this);
    mainLayout->addWidget(studyParameterListWidget_);

    QHBoxLayout* okayCancelButtonLayout = new QHBoxLayout;
    okayCancelButtonLayout->setAlignment(Qt::AlignRight);

    QDialogButtonBox* okayCancelbuttonBox_ = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(okayCancelbuttonBox_, SIGNAL(accepted()), this, SLOT(clickedOkayButton()));
    connect(okayCancelbuttonBox_, SIGNAL(rejected()), this, SLOT(clickedCancelButton()));
    okayCancelButtonLayout->addWidget(okayCancelbuttonBox_);
    mainLayout->addLayout(okayCancelButtonLayout);
}

void StudyParameterDialog::clickedOkayButton() {    
    accept();    
}

void StudyParameterDialog::clickedCancelButton() {
    reject();
}

}

