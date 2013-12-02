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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_RANGESLIDERQT_H
#define IVW_RANGESLIDERQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QSplitter>
#include <QResizeEvent>

namespace inviwo {

class IVW_QTWIDGETS_API RangeSliderQt : public QSplitter {
    Q_OBJECT

public:
    RangeSliderQt(Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = NULL);
    virtual ~RangeSliderQt();

public slots:
    int minValue();
    int maxValue();
    int minRange();
    int maxRange();

    void setValue(int, int);
    void setMinValue(int);
    void setMaxValue(int);

    void setRange(int, int);
    void setMinRange(int);
    void setMaxRange(int);

signals:
    void valuesChanged(int min, int max);
    void valuesSet(int min, int max);

protected:
    int constrainValues(int idx);
    void resizeEvent(QResizeEvent *event);

    int fromInternalToExternal(int val);
    int fromExternalToInternal(int val);

    int getHandleWidth();

protected slots:
    void updateSplitterPosition(int pos, int idx);

private:
    int internalRange_[2];
    int internalValue_[2];

    int range_[2];
    int value_[2];
};

}//namespace

#endif // IVW_RANGESLIDERQT_H