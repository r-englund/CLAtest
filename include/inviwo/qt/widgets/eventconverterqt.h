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

#ifndef IVW_EVENTCONVERTERQT_H
#define IVW_EVENTCONVERTERQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QMouseEvent>
#include <QInputEvent>
#include <inviwo/core/interaction/events/mouseevent.h>

namespace inviwo {

class IVW_QTWIDGETS_API EventConverterQt {
public:
    EventConverterQt();
    ~EventConverterQt();

    static MouseEvent::MouseButton getMouseButton(QMouseEvent* e);
    static InteractionEvent::Modifier getModifier(QInputEvent* e);
	static char getKeyButton(QKeyEvent* e);
};
} // namespace
#endif // IVW_EVENTCONVERTERQT_H