/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Hans-Christian Helltegen, Timo Ropinski
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/eventconverterqt.h>

namespace inviwo {

EventConverterQt::EventConverterQt() {}
EventConverterQt::~EventConverterQt() {}

char EventConverterQt::getKeyButton(QKeyEvent* e) {
    char key = toupper(e->nativeVirtualKey());

    if ((key >= '0' && key <= '9')||(key >= 'A' && key <= 'Z'))
        return key;
    else
        return 0;
}

MouseEvent::MouseButton inviwo::EventConverterQt::getMouseButton(QMouseEvent* e) {
    if (e->buttons() == Qt::LeftButton)
        return MouseEvent::MOUSE_BUTTON_LEFT;
    else if (e->buttons() == Qt::RightButton)
        return MouseEvent::MOUSE_BUTTON_RIGHT;
    else if (e->buttons() == Qt::MiddleButton)
        return MouseEvent::MOUSE_BUTTON_MIDDLE;

    return MouseEvent::MOUSE_BUTTON_NONE;
}

MouseEvent::MouseButton inviwo::EventConverterQt::getMouseWheelButton(QWheelEvent* e) {
    if (e->buttons() == Qt::LeftButton)
        return MouseEvent::MOUSE_BUTTON_LEFT;
    else if (e->buttons() == Qt::RightButton)
        return MouseEvent::MOUSE_BUTTON_RIGHT;
    else if (e->buttons() == Qt::MiddleButton)
        return MouseEvent::MOUSE_BUTTON_MIDDLE;

    return MouseEvent::MOUSE_BUTTON_NONE;
}

InteractionEvent::Modifier inviwo::EventConverterQt::getModifier(QInputEvent* e) {
    if (e->modifiers() == Qt::ShiftModifier)
        return InteractionEvent::MODIFIER_SHIFT;
    else if (e->modifiers() == Qt::ControlModifier)
        return InteractionEvent::MODIFIER_CTRL;
    else if (e->modifiers() == Qt::AltModifier)
        return InteractionEvent::MODIFIER_ALT;

    return InteractionEvent::MODIFIER_NONE;
}

} // namespace