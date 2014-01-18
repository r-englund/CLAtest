/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/properties/property.h>
#include <inviwo/core/network/processornetwork.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
#include <inviwo/qt/widgets/properties/collapsiblegroupboxwidgetqt.h>
#include <inviwo/qt/widgets/propertyselectiontreewidget.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <apps/inviwo/inviwomainwindow.h>
#include <QDialogButtonBox>

namespace inviwo {


void PropertySelectionTree::mousePressEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton)
        dragStartPosition_ = e->pos();
    else if (e->buttons() & Qt::RightButton) {
        QTreeWidgetItem* selectedItem = itemAt(e->pos());        
        if (selectedItem && !selectedItem->parent()) {            
            
            QMenu menu;
            QAction* deleteWorkspace = menu.addAction(tr("Delete"));
            QAction* result = menu.exec(QCursor::pos());            
            if (result == deleteWorkspace) {
                delete selectedItem;
            }            
        }
    }
    QTreeWidget::mousePressEvent(e);
}

void PropertySelectionTree::mouseMoveEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton) {
        if ((e->pos() - dragStartPosition_).manhattanLength() < QApplication::startDragDistance())
            return;

        QTreeWidgetItem* selectedProperty = itemAt(dragStartPosition_);
        if (selectedProperty && selectedProperty->parent()) {
            //create drag object
        }
           
    }
}

PropertySelectionTreeWidget::PropertySelectionTreeWidget() : QWidget() {
    setObjectName("PropertySelectionTreeWidget");   
  
    vLayout_ = new QVBoxLayout();
    propertySelectionTree_ = new PropertySelectionTree(this);
    propertySelectionTree_->setHeaderHidden(true);
    propertySelectionTree_->setExpandsOnDoubleClick(false);
    vLayout_->addWidget(propertySelectionTree_);
    setLayout(vLayout_);
}

PropertySelectionTreeWidget::~PropertySelectionTreeWidget() {}

void PropertySelectionTreeWidget::addProcessorNetwork(ProcessorNetwork* processorNetwork, std::string workspaceFileName) { 
    std::vector<Processor*> processors = processorNetwork->getProcessors();    
    QTreeWidgetItem* worksapceItem = new QTreeWidgetItem(QStringList(QString::fromStdString(workspaceFileName)));
    if (processors.size()) {       
        propertySelectionTree_->addTopLevelItem(worksapceItem);        
    }
    else {
        LogWarn("Empty workpace with no processors" << workspaceFileName);
        return;
    }

    for (size_t i=0; i<processors.size(); i++) {
        std::vector<Property*> properties = processors[i]->getProperties();

        std::string processorId = processors[i]->getIdentifier();
        QTreeWidgetItem* processorItem = new QTreeWidgetItem(QStringList(QString::fromStdString(processorId)));

        worksapceItem->addChild(processorItem);

        for (size_t j=0; j<properties.size(); j++) {
            std::string id = properties[j]->getIdentifier();                
            QTreeWidgetItem* newItem = new QTreeWidgetItem(QStringList(QString::fromStdString(id)));
            processorItem->addChild(newItem);
            newItem->setFlags(newItem->flags() | Qt::ItemIsUserCheckable);
            newItem->setCheckState(0, Qt::Unchecked);  
        }

        propertySelectionTree_->addTopLevelItem(processorItem);
        processorItem->sortChildren(0, Qt::AscendingOrder);
    }        
    propertySelectionTree_->expandAll();
}

void PropertySelectionTreeWidget::clear() {
    propertySelectionTree_->clear();
}

std::vector<Property*> PropertySelectionTreeWidget::getSelectedProperties(ProcessorNetwork* processorNetwork) {
    std::vector<Property*> selectedProperties;
    int workspaceCount  = propertySelectionTree_->topLevelItemCount();    
    for (int i=0; i<workspaceCount; i++) {
        QTreeWidgetItem* wrokspaceItem = propertySelectionTree_->topLevelItem(i);
        int processorCount = wrokspaceItem->childCount();      

        for (int j=0; j<processorCount; j++) {
            QTreeWidgetItem* processorItem = wrokspaceItem->child(j);

            int propertyCount = processorItem->childCount();      

            QString qproecessorId = processorItem->text(0);
            std::string proecessorId = qproecessorId.toLocal8Bit().constData();

             for (int k=0; k<propertyCount; k++) {

                QTreeWidgetItem* propertyItem = processorItem->child(k);
                if (propertyItem->checkState(0) == Qt::Checked) {
                    QString propertyId = propertyItem->text(0);
                    std::string prop = propertyId.toLocal8Bit().constData();

                    QString workspaceName = wrokspaceItem->text(0);
                    std::string workspace = workspaceName.toLocal8Bit().constData();

                    Processor* processor = processorNetwork->getProcessorByName(proecessorId);

                    Property* selectedProperty = processor->getPropertyByIdentifier(prop);

                    if (selectedProperty) 
                        selectedProperties.push_back(selectedProperty);
                }
             }
        }
    }
    return selectedProperties;
}


PropertySelectionTreeDialog::PropertySelectionTreeDialog(ProcessorNetwork* processorNetwork,
                                                         std::vector<Property*> &selectedProperty,
                                                         QWidget* parent) 
                                                         : QDialog(parent), selectedProperties_(selectedProperty),
                                                           processorNetwork_(processorNetwork)
{    
    initDialog();
    selectionTree_->addProcessorNetwork(processorNetwork);
}

void PropertySelectionTreeDialog::initDialog() {
    std::string title = std::string("Property Selection");
    setWindowTitle(tr(title.c_str()));

    QSize rSize(384,512);
    setFixedSize(rSize);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    selectionTree_ = new PropertySelectionTreeWidget();
    mainLayout->addWidget(selectionTree_);

    QHBoxLayout* okayCancelButtonLayout = new QHBoxLayout;
    okayCancelButtonLayout->setAlignment(Qt::AlignRight);

    QDialogButtonBox* okayCancelbuttonBox_ = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(okayCancelbuttonBox_, SIGNAL(accepted()), this, SLOT(clickedOkayButton()));
    connect(okayCancelbuttonBox_, SIGNAL(rejected()), this, SLOT(clickedCancelButton()));
    okayCancelButtonLayout->addWidget(okayCancelbuttonBox_);
    mainLayout->addLayout(okayCancelButtonLayout);
}

void PropertySelectionTreeDialog::clickedOkayButton() {    
    accept();
    selectedProperties_ =  selectionTree_->getSelectedProperties(processorNetwork_);
}

void PropertySelectionTreeDialog::clickedCancelButton() {
    reject();
}



} // namespace