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
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

#ifndef IVW_COLORPROPERTYWDIGETQT_H
#define IVW_COLORPROPERTYWDIGETQT_H

#include <QColorDialog>
#include <QLabel>
#include <QPushButton>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/buttonpropertywidgetqt.h>
#include <inviwo/core/properties/buttonproperty.h>

namespace inviwo {

class IVW_QTWIDGETS_API ColorPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:

    ColorPropertyWidgetQt(Property* property);
    virtual ~ColorPropertyWidgetQt();

    void updateFromProperty();
    QColor getCurrentColor();

private:
    Property* property_;
    QPushButton* btnColor_;
    QColorDialog* colorDialog_;
    QColor* currentColor_;
    EditableLabelQt* label_;

    void generateWidget();

public slots:
    void setPropertyValue();
    void openColorDialog();
    void setPropertyDisplayName();
};

}//namespace

#endif //IVW_COLORPROPERTYWIDGETQT_H