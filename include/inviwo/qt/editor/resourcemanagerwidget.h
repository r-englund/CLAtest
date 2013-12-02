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

#ifndef IVW_RESOURCE_MANAGER_WIDGET_H
#define IVW_RESOURCE_MANAGER_WIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/qt/widgets/inviwodockwidget.h>
#include <inviwo/core/resources/resourcemanagerobserver.h>


#include <QFrame>
#include <QVBoxLayout>
#include <QListView>
#include <QStandardItemModel>
#include <QKeyEvent>

namespace inviwo {

class IVW_QTEDITOR_API ResourceManagerWidget : public InviwoDockWidget, public ResourceManagerObserver {
    Q_OBJECT
public:
    ResourceManagerWidget(QWidget* parent);
    ~ResourceManagerWidget();
    void resourceAdded(const Resource* resource);	
    void resourceRemoved(const Resource* resource);	
    void keyPressEvent(QKeyEvent *event);
private:
	void buildLayout();
    void removeSelectedItems();

    QListView* listView_;
    QStandardItemModel *model_;
public slots:


};

} // namespace

#endif // IVW_RESOURCE_MANAGER_WIDGET_H