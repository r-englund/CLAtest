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

#ifndef IVW_BUTTONPROPERTYWIDGETQT_H
#define IVW_BUTTONPROPERTYWIDGETQT_H

#include <QPushButton>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/buttonproperty.h>



namespace inviwo {


class ButtonPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:
    ButtonPropertyWidgetQt(ButtonProperty* property);

    void updateFromProperty();
    QPushButton* getButton();
private:
    ButtonProperty* property_;
    QPushButton* button_;

    void generateWidget();

public slots:
    void handleButton();
};

} //namespace

#endif // IVW_BUTTONPROPERTYWIDGETQT_H