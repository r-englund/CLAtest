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



} // namespace