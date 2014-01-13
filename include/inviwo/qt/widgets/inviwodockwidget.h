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

#ifndef IVW_INVIWODOCKWIDGET_H
#define IVW_INVIWODOCKWIDGET_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QDockWidget>

namespace inviwo {

class IVW_QTWIDGETS_API InviwoDockWidget : public QDockWidget {
    Q_OBJECT

public:
    InviwoDockWidget(QString title, QWidget* parent);
    virtual ~InviwoDockWidget();
};

} // namespace

#endif // IVW_INVIWODOCKWIDGET_H