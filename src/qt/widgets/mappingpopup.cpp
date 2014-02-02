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

#include <inviwo/qt/widgets/mappingpopup.h>

namespace inviwo {

MappingPopup::MappingPopup(EventProperty* eventProperty, EventPropertyManager* parentManager) {
    parentManager_ = parentManager;
    eventProperty_ = eventProperty;
}

// Sends the first event back to the property and closes the window
void MappingPopup::mousePressEvent(QMouseEvent* event) {
    parentManager_->changeMouseMapping(eventProperty_, EventConverterQt::getMouseButton(event), EventConverterQt::getModifier(event));
}

void MappingPopup::keyReleaseEvent(QKeyEvent* event) {
    char button = EventConverterQt::getKeyButton(event);

    if (button > 0) {
        parentManager_->changeKeyMapping(eventProperty_, button, EventConverterQt::getModifier(event));
        this->close();
    }
}

void MappingPopup::mouseReleaseEvent(QMouseEvent* event) {
    this->close();
}

} // namespace