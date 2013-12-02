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

#ifndef IVW_FLOATMAT3ROPERTYWIDGETQT_H
#define IVW_FLOATMAT3ROPERTYWIDGETQT_H

#include <QLineEdit>


#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/editablelabelqt.h>

#include <inviwo/core/properties/matrixproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API FloatMat3PropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:
    FloatMat3PropertyWidgetQt(FloatMat3Property* property);

    void updateFromProperty();

private:
    FloatMat3Property* property_;
    QLineEdit* mat0x0_;
    QLineEdit* mat0x1_;
    QLineEdit* mat0x2_;
    QLineEdit* mat1x0_;
    QLineEdit* mat1x1_;
    QLineEdit* mat1x2_;
    QLineEdit* mat2x0_;
    QLineEdit* mat2x1_;
    QLineEdit* mat2x2_;

    EditableLabelQt* label_;

    void generateWidget();

    public slots:
        void set0x0Value();
        void set0x1Value();
        void set0x2Value();
        void set1x0Value();
        void set1x1Value();
        void set1x2Value();
        void set2x0Value();
        void set2x1Value();
        void set2x2Value();

        void setPropertyValue();
        void setPropertyDisplayName();
};

} // namespace

#endif // IVW_FLOATMAT3ROPERTYWIDGETQT_H