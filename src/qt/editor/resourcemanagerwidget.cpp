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
    model_ = new QStandardItemModel();
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
    // Destruction of QModelIndexList creates a segmentation error for some reason (Qt 4.8.1)
    listView_->setUpdatesEnabled(false);


    QModelIndexList indexes = listView_->selectionModel()->selectedIndexes();
    qSort(indexes.begin(), indexes.end(), qGreater<QModelIndex>());

    for(QModelIndexList::const_iterator iter = indexes.constBegin(); iter != indexes.constEnd(); ++iter){
        ResourceManager::instance()->removeResource(model_->item(iter->row())->text().toStdString());
        //model_->removeRow(iter->row());
    }
    listView_->setUpdatesEnabled(true);
}



} // namespace