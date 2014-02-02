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

#include <inviwo/qt/widgets/inviwodockwidget.h>

namespace inviwo {

InviwoDockWidget::InviwoDockWidget(QString title, QWidget* parent) : QDockWidget(title, parent) {
}

InviwoDockWidget::~InviwoDockWidget() {}

} // namespace