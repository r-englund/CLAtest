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

#include <inviwo/qt/widgets/eventconverterqt.h>

namespace inviwo {

EventConverterQt::EventConverterQt() {}
EventConverterQt::~EventConverterQt() {}

char EventConverterQt::getKeyButton( QKeyEvent* e ){
	char key = toupper(e->nativeVirtualKey());
	if ((key >= '0' && key <= '9' )||(key >= 'A' && key <= 'Z'))
		return key;
	else
		return 0;
}

MouseEvent::MouseButton inviwo::EventConverterQt::getMouseButton( QMouseEvent* e ) {
    if (e->buttons() == Qt::LeftButton) {
        return MouseEvent::MOUSE_BUTTON_LEFT;
    } else if (e->buttons() == Qt::RightButton) {
        return MouseEvent::MOUSE_BUTTON_RIGHT;
    } else if (e->buttons() == Qt::MiddleButton) {
        return MouseEvent::MOUSE_BUTTON_MIDDLE;
    }
    return MouseEvent::MOUSE_BUTTON_NONE;
}

InteractionEvent::Modifier inviwo::EventConverterQt::getModifier( QInputEvent* e ) {
    if (e->modifiers() == Qt::ShiftModifier) {
        return InteractionEvent::MODIFIER_SHIFT;
    } else if (e->modifiers() == Qt::ControlModifier) {
        return InteractionEvent::MODIFIER_CTRL;
    } else if (e->modifiers() == Qt::AltModifier) {
        return InteractionEvent::MODIFIER_ALT;
    }
    return InteractionEvent::MODIFIER_NONE;
}

} // namespace