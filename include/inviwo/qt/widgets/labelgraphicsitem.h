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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_LABELGRAPHICSITEM_H
#define IVW_LABELGRAPHICSITEM_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QGraphicsTextItem>
#include <inviwo/core/util/observer.h>

namespace inviwo {

class IVW_QTWIDGETS_API LabelGraphicsItem : public QGraphicsTextItem, public VoidObservable {

public:
    LabelGraphicsItem(QGraphicsItem* parent);
    ~LabelGraphicsItem();

    QString text() const;
    void setText(const QString&);

    QString croppedText() const;
    void setCrop(int, int);
    bool isCropped() const;

    void setNoFocusOut();
    bool isFocusOut() const;

protected:
    bool doCrop(const QString& str);
    void updateCrop();

    void keyPressEvent(QKeyEvent* event);
    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);

private:
    int maxBefore_;
    int maxAfter_;
    bool focusOut_;
    QString orgText_;
};

} // namespace

#endif // IVW_LABELGRAPHICSITEM_H