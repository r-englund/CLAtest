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

#ifndef IVW_FLOATMAT2PROPERTYWIDGETQT_H
#define IVW_FLOATMAT2PROPERTYWIDGETQT_H

#include <QLineEdit>


#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/core/properties/matrixproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API FloatMat2PropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:
    FloatMat2PropertyWidgetQt(FloatMat2Property* property);

    void updateFromProperty();

private:
    FloatMat2Property* property_;
    QLineEdit* mat0x0_; ///< coordinates 0,0 of the matrix equals upper left corner
    QLineEdit* mat0x1_;
    QLineEdit* mat1x0_;
    QLineEdit* mat1x1_;///< coordinates 1,1 of the matrix equals lower right corner
    EditableLabelQt* label_;
    

    void generateWidget();

    public slots:
        void set0x0Value();
        void set0x1Value();
        void set1x0Value();
        void set1x1Value();
        void setPropertyValue();
        void setPropertyDisplayName();
};

} // namespace

#endif // IVW_FLOATMAT2PROPERTYWIDGETQT_H