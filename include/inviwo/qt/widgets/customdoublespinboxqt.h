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

#ifndef IVW_DOUBLE_SPIN_BOX_QT_H
#define IVW_DOUBLE_SPIN_BOX_QT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QDoubleSpinBox>


namespace inviwo {

/** \class CustomDoubleSpinBoxQt 
 *
 * Enables displaying a value with a certain precision without truncation the actual value, which QDoubleSpinBox does.
 */
class IVW_QTWIDGETS_API CustomDoubleSpinBoxQt : public QDoubleSpinBox {
     Q_OBJECT
public:
    explicit CustomDoubleSpinBoxQt(QWidget *parent = 0);
    virtual QString textFromValue(double value) const;

    /**
     * Override QDoubleSpinBox size hint so that 
     * it does not use the one for many decimals.
     * 
     */
    QSize sizeHint() const { return cachedSizeHint_; }
    /**
     * Override QDoubleSpinBox decimals so that 
     * we can cache size and decimals to display.
     * 
     */
    virtual void setDecimals(int decimals);
protected:
    QSize cachedSizeHint_;
    int displayDecimals_;

};

}//namespace

#endif // IVW_DOUBLE_SPIN_BOX_QT_H