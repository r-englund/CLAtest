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

#ifndef IVW_LIGHTPROPERTYWIDGETQT_H
#define IVW_LIGHTPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/lightpositionwidgetqt.h>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/vectorproperties.h>
#include <QtCore/qmath.h>

namespace inviwo {

class IVW_QTWIDGETS_API LightPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:
    LightPropertyWidgetQt(FloatVec3Property* property);
    virtual ~LightPropertyWidgetQt();

    void updateFromProperty();

private:
    FloatVec3Property* property_;
    LightPositionWidgetQt* lightWidget_;
    EditableLabelQt* label_;

    void generateWidget();

public slots:
    void setPropertyValue();
    void setPropertyDisplayName();
};

} // namespace

#endif // IVW_LightPropertyWidgetQt_H