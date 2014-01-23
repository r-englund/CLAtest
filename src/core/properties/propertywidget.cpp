/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include <inviwo/core/properties/propertywidget.h>

namespace inviwo {

PropertyWidget::PropertyWidget()
    : property_(0) {
}

PropertyWidget::PropertyWidget(Property* property)
    : property_(property) {
}

PropertyWidget::~PropertyWidget() {
}

Property* PropertyWidget::getProperty() {
    return property_;
}

void PropertyWidget::setProperty(Property* property) {
    property_ = property;
}


} // namespace
