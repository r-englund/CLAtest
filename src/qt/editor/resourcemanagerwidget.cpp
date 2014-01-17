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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#include <inviwo/qt/editor/resourcemanagerwidget.h>
#include <inviwo/core/resources/resourcemanager.h>

#include <QStandardItem>
#include <string>

namespace inviwo {

ResourceManagerWidget::ResourceManagerWidget(QWidget* parent) : InviwoDockWidget(tr("Resources"), parent), ResourceManagerObserver() {
    setObjectName("ResourceManagerWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	ResourceManager::instance()->addObserver(this);
	buildLayout();	
}

ResourceManagerWidget::~ResourceManagerWidget() {}


void ResourceManagerWidget::buildLayout() {
	// Components needed for layout
	QFrame* frame_ = new QFrame();
	QVBoxLayout* mainLayout = new QVBoxLayout();
    listView_ = new QListView();
    model_ = new QStandardItemModel(listView_);
    listView_->setModel(model_);  

    
    mainLayout->addWidget(listView_);
	frame_->setLayout(mainLayout);
    
	setWidget(frame_);
	
}

void ResourceManagerWidget::resourceAdded( const Resource* resource )
{
    QStandardItem *item = new QStandardItem( QString::fromStdString( resource->getIdentifier() ) );
    item->setEditable( false );  
    model_->appendRow( item );
}

void ResourceManagerWidget::resourceRemoved( const Resource* resource )
{
    QList<QStandardItem *> foundItem = model_->findItems( QString::fromStdString( resource->getIdentifier() ) );
    for(QList<QStandardItem *>::iterator it = foundItem.begin(); it != foundItem.end(); ++it) {
        model_->removeRow(model_->indexFromItem(*it).row());
    }    
}

void ResourceManagerWidget::keyPressEvent( QKeyEvent *event )
{
    if(event->key() == Qt::Key_Delete) {
        removeSelectedItems();
    }
}

void ResourceManagerWidget::removeSelectedItems()
{    
    listView_->setUpdatesEnabled(false);
    
    //Destruction of QModelIndexList creates a segmentation error for some reason (Qt 4.8.1)
    //QModelIndexList indexes = listView_->selectionModel()->selectedIndexes();
    std::vector<int> rows;    
    for (int i=0; i<model_->rowCount(); i++)
        if (listView_->selectionModel()->isRowSelected(i, QModelIndex())) rows.push_back(i);   
    
    qSort(rows.begin(), rows.end(), qGreater<int>());

    for (size_t i=0; i<rows.size(); i++) {
        //FIXME:: QString::toStdString() crashes hence toLatin1() used.
        std::string resrcIdentifier(model_->item(rows[i])->text().toLatin1().data());
        ResourceManager::instance()->removeResource(resrcIdentifier);        
    }
    listView_->setUpdatesEnabled(true);
}



} // namespace