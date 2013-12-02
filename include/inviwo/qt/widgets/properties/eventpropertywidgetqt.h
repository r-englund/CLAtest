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

#ifndef IVW_EVENTPROPERTYWIDGETQT_H
#define IVW_EVENTPROPERTYWIDGETQT_H

#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/qt/widgets/eventpropertymanager.h>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/mappingpopup.h>
#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

namespace inviwo {

class IVW_QTWIDGETS_API EventPropertyWidgetQt : public PropertyWidgetQt {
    Q_OBJECT

public:
    EventPropertyWidgetQt(EventProperty* eventproperty);
    void updateFromProperty();
	void setManager(EventPropertyManager* eventPropertyManager) { eventPropertyManager_ = eventPropertyManager; };

private:
    EventProperty* eventproperty_;
    QPushButton* button_;
	EventPropertyManager* eventPropertyManager_;

    void generateWidget();

public slots:
    void clickedSlot();
};

} //namespace

#endif // IVW_EVENTPROPERTYWIDGETQT_H