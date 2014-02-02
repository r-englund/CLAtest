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

#ifndef IVW_PROCESSORPROPERTYWIDGET_H
#define IVW_PROCESSORPROPERTYWIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QWidget>

namespace inviwo {

class IVW_QTEDITOR_API ProcessorPropertyWidget : public QWidget {
    Q_OBJECT

public:
    ProcessorPropertyWidget(QWidget* parent);
    ~ProcessorPropertyWidget();
};

} // namespace

#endif // IVW_PROCESSORPROPERTYWIDGET_H