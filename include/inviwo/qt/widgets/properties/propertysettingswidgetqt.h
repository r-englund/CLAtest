/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file author: Alexander Johansson
 *
 *********************************************************************************/

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
#include <inviwo/core/properties/ordinalproperty.h>




namespace inviwo {
class IVW_QTWIDGETS_API PropertySettingsWidgetQt : public QWidget {

    Q_OBJECT
public:
    PropertySettingsWidgetQt(Property*, QWidget*);
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
     */
     void generateWidget();


    void generateSingleWidget();
    /**
     * \brief Generates a 2D-vector widget
     *
     * Generates a 2D-vector widget, it is called from the generatewidget method in the constructor
     *
     */
     void generateVec2Widget();

    /**
     * \brief Generates a 3D-vector widget
     *
     * Generates a 3D-vector widget, it is called from the generatewidget method in the constructor
     *
     */void generateVec3Widget();

    /**
     * \brief Generates a 4D-vector widget
     *
     *Generates a 4D-vector widget, it is called from the generatewidget method in the constructor
     *
     */void generateVec4Widget();

public slots:

    /**
     * \brief Saves the values from the widget
     *
     */
    void save();


    /**
     * \brief Updates the values in the line edits of the widget
     */
    void reload();


    /**
     * \brief Hides the widget and discard the changes
     *
     */
    void cancel();

    void fixTabOrder();
};

}//namespace

#endif //IVW_PROPERTYSETTINGSWIDGETQT_H



