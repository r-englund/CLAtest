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

#ifndef IVW_OPTIONPROPERTYWIDGETQT_H

#define IVW_OPTIONPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>

#include <QComboBox>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <typeinfo>

namespace inviwo {

class IVW_QTWIDGETS_API OptionPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:

    OptionPropertyWidgetQt(BaseOptionProperty* property);

    void updateFromProperty();

private:

    BaseOptionProperty* property_;
    QComboBox* comboBox_;
    EditableLabelQt* label_;
    void generateWidget();
    void fillComboBox();
    bool updating_;

public slots:
     void optionChanged();
     void setPropertyDisplayName();
     
};

} // namespace

#endif // IVW_OPTIONPROPERTYWIDGETQT_H