#ifndef IVW_RESOURCE_MANAGER_WIDGET_H
#define IVW_RESOURCE_MANAGER_WIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/qt/editor/inviwodockwidget.h>
#include <inviwo/core/resources/resourcemanagerobserver.h>


#include <QFrame>
#include <QVBoxLayout>
#include <QListView>
#include <QStandardItemModel>

namespace inviwo {

class IVW_QTEDITOR_API ResourceManagerWidget : public InviwoDockWidget, public ResourceManagerObserver {
    Q_OBJECT
public:
    ResourceManagerWidget(QWidget* parent);
    ~ResourceManagerWidget();
    void resourceAdded(const Resource* resource);	
    void resourceRemoved(const Resource* resource);	

private:
	void buildLayout();

    QListView* listView_;
    QStandardItemModel *model_;
public slots:


};

} // namespace

#endif // IVW_RESOURCE_MANAGER_WIDGET_H