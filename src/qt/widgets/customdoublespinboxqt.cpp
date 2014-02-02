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

#include <inviwo/qt/widgets/customdoublespinboxqt.h>
#include <limits>

namespace inviwo {



CustomDoubleSpinBoxQt::CustomDoubleSpinBoxQt(QWidget* parent /*= 0*/) {
    // Enables setting number of decimals to display
    displayDecimals_ = decimals();
    // Save default sizeHint before changing decimal property
    cachedSizeHint_ = QDoubleSpinBox::sizeHint();
    // Set decimals to large value so that QDoubleSpinBox does not truncate the value
    QDoubleSpinBox::setDecimals(std::numeric_limits<double>::max_exponent);
}

QString CustomDoubleSpinBoxQt::textFromValue(double value) const {
    return QString::number(value, 'f', displayDecimals_);
}

void CustomDoubleSpinBoxQt::setDecimals(int decimals) {
    if (decimals == displayDecimals_)
        return;

    displayDecimals_ = decimals;
    // Block so that no signals are sent
    // since the value does not change
    blockSignals(true);
    double val = value();
    QDoubleSpinBox::setDecimals(decimals);
    cachedSizeHint_ = QDoubleSpinBox::sizeHint();
    QDoubleSpinBox::setDecimals(std::numeric_limits<double>::max_exponent);
    setValue(val);
    blockSignals(false);
}

} // namespace inviwo
