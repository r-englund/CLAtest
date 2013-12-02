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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_DIRECTORYPROPERTYWIDGETQT_H
#define IVW_DIRECTORYPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <QLineEdit>
#include <QToolButton>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/properties/directoryproperty.h>
#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/stringpropertywidgetqt.h>

namespace inviwo {

class IVW_QTWIDGETS_API DirectoryPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:
    DirectoryPropertyWidgetQt(DirectoryProperty* property);

    void updateFromProperty();

private:
    DirectoryProperty* property_;
    QLineEdit* lineEdit_;
    QToolButton* openButton_;
    EditableLabelQt* directoryLabel_;
    void generateWidget();

public slots:
    void setPropertyValue();
    void setPropertyTreeInfo(std::string path);
    void setPropertyDisplayName();
};

} // namespace

#endif // IVW_DIRECTORYPROPERTYWIDGETQT_H