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

#ifndef IVW_PROPERTYSETTINGSWIDGETQT_H
#define IVW_PROPERTYSETTINGSWIDGETQT_H

//Qt
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
//Core
#include <inviwo/core/properties/scalarproperties.h>
#include <inviwo/core/properties/vectorproperties.h>



namespace inviwo {
class IVW_QTWIDGETS_API PropertySettingsWidgetQt : public QWidget {

    Q_OBJECT
public:
    PropertySettingsWidgetQt(Property *property);
    virtual ~PropertySettingsWidgetQt();
  
private:
    Property* property_;
    FloatProperty* floatProperty_; ///< Property  for the 1-D float vectors
    IntProperty* intProperty_; ///< Property for the 1-D integer vectors
    FloatVec2Property* floatVec2Property_; ///< Property for the 2-D float vectors
    FloatVec3Property* floatVec3Property_; ///< Property for the 3-D float vectors
    FloatVec4Property* floatVec4Property_; ///< Property for the 4-D float vectors
    IntVec2Property* intVec2Property_; ///< Property for the 2-D integer vectors
    IntVec3Property* intVec3Property_; ///< Property for the 3-D integer vectors
    IntVec4Property* intVec4Property_; ///< Property for the 4-D integer vectors

    QLineEdit* lineEditMaxX_; ///< Text box for the maximum value of the X component 
    QLineEdit* lineEditMaxY_; ///< Text box for the maximum value of the Y component 
    QLineEdit* lineEditMaxZ_; ///< Text box for the maximum value of the Z component 
    QLineEdit* lineEditMaxW_; ///< Text box for the maximum value of the W component 

    QLineEdit* lineEditMinX_; ///< Text box for the minimum value of the X component 
    QLineEdit* lineEditMinY_; ///< Text box for the minimum value of the Y component 
    QLineEdit* lineEditMinZ_; ///< Text box for the minimum value of the Z component 
    QLineEdit* lineEditMinW_; ///< Text box for the minimum value of the W component 

    QLineEdit* lineEditIcrementX_; ///< Text box for the increment value of the X component
    QLineEdit* lineEditIcrementY_; ///< Text box for the increment value of the Y component
    QLineEdit* lineEditIcrementZ_; ///< Text box for the increment value of the Z component
    QLineEdit* lineEditIcrementW_; ///< Text box for the increment value of the W component

    QLabel* xValueLabel_;
    QLabel* yValueLabel_;
    QLabel* zValueLabel_;
    QLabel* wValueLabel_;

    QLabel* minValueLabel_;
    QLabel* maxValueLabel_;
    QLabel* incValueLabel_;

    QGridLayout* gridLayout_;
    QPushButton* btnApply_; 
    QPushButton* btnCancel_;

    /** 
     * \brief Generates the widget
     *
     * Generates the appropriate widget depending on the property in the constructor
     * 
     * @return void <DESCRIBE ME>
     */void generateWidget();

    /** 
     * \brief Generates a 2D-vector widget
     *
     * Generates a 2D-vector widget, it is called from the generatewidget method in the constructor
     * 
     * @return void <DESCRIBE ME>
     */void generateVec2Widget();

    /** 
     * \brief Generates a 3D-vector widget
     *
     * Generates a 3D-vector widget, it is called from the generatewidget method in the constructor
     * 
     * @return void <DESCRIBE ME>
     */void generateVec3Widget();

    /** 
     * \brief Generates a 4D-vector widget
     *
     *Generates a 4D-vector widget, it is called from the generatewidget method in the constructor
     * 
     * @return void <DESCRIBE ME>
     */void generateVec4Widget();

public slots:

    /** 
     * \brief Saves the values from the widget
     *
     * <DESCRIBE THE METHOD>
     * 
     * @return void <DESCRIBE ME>
     */void save();


    /** 
     * \brief Updates the values in the line edits of the widget
     *
     * <DESCRIBE THE METHOD>
     * 
     * @return void <DESCRIBE ME>
     */void reload();


    /** 
     * \brief Hides the widget and discard the changes
     *
     * <DESCRIBE THE METHOD>
     * 
     * @return void <DESCRIBE ME>
     */void cancel();

};

}//namespace

#endif //IVW_PROPERTYSETTINGSWIDGETQT_H



