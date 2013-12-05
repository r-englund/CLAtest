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

#ifndef IVW_CAMERAPROPERTYWIDGETQT_H
#define IVW_CAMERAPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/properties/collapsivegroupboxwidgetqt.h>
#include <QSlider>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/cameraproperty.h>

namespace inviwo {

class IVW_QTWIDGETS_API CameraPropertyWidgetQt : public PropertyWidgetQt {
    Q_OBJECT

public:
    CameraPropertyWidgetQt(CameraProperty* property);
    void updateFromProperty();

public slots:
    void setPropertyValue();
    void setPropertyDisplayName();
    void visibilityModified(int mode);

private:
    CameraProperty* property_;
    //QSlider* slider_;
    EditableLabelQt* label_;
    CollapsiveGroupBoxWidgetQt* collapsiveGroupBoxWidget_;
    void generateWidget();
    std::vector<PropertyWidgetQt*> subPropertyWidgets_;

};

} // namespace

#endif // IVW_CAMERAPROPERTYWIDGETQT_H