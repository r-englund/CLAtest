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
 * Primary author : Hans-Christian Helltegen
 *
 **********************************************************************/

#ifndef IVW_EVENTPROPERTYMANAGERWIDGET_H
#define IVW_EVENTPROPERTYMANAGERWIDGET_H

#include <inviwo/core/util/observer.h>
#include <inviwo/qt/widgets/eventpropertymanager.h>
#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>

#include <QVBoxLayout>
#include <QWidget>

namespace inviwo {

class IVW_QTWIDGETS_API EventPropertyManagerWidget : public QWidget, public VoidObserver {
	Q_OBJECT

public:
	EventPropertyManagerWidget(EventPropertyManager* eventPropertyManager);
	~EventPropertyManagerWidget();	

	void notify();
private:
	void emptyLayout(QVBoxLayout* layout);
	void drawEventPropertyWidgets();	

	EventPropertyManager* eventPropertyManager_;
	QVBoxLayout* mainLayout_;
	std::map<std::string, bool> groupCollapsed;
};

} //namespace

#endif // IVW_EVENTPROPERTYMANAGERWIDGET_H