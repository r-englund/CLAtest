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

#ifndef IVW_MAPPINGPOPUP_H
#define IVW_MAPPINGPOPUP_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/qt/widgets/eventconverterqt.h>
#include <inviwo/qt/widgets/eventpropertymanager.h>

#include <QWidget>

namespace inviwo {

class IVW_QTWIDGETS_API MappingPopup : public QWidget {
    Q_OBJECT

public:
    MappingPopup(EventProperty* eventProperty, EventPropertyManager* parentManager);

    void keyReleaseEvent(QKeyEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);

private:
    EventProperty* eventProperty_;
    EventPropertyManager* parentManager_;
};

} // namespace

#endif // IVW_MAPPINGPOPUP_H