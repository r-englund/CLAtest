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
 * Main file authors: Alexander Johansson, Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_FLOATSLIDERQT_H
#define IVW_FLOATSLIDERQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/customsliderwidgetqt.h>
#include <inviwo/qt/widgets/customdoublespinboxqt.h>
#include <QSlider>
#include <QHBoxLayout>
#include <math.h>
#include <sstream>

namespace inviwo {

class IVW_QTWIDGETS_API FloatSliderWidgetQt : public QWidget {
    Q_OBJECT
public:

    FloatSliderWidgetQt();

    virtual ~FloatSliderWidgetQt();

    /**
     * \brief Returns the float value of the slider
     *
     * <DESCRIBE THE METHOD>
     *
     * @return float the current value of the slider
     */
    float getValue();
    /**
     * \brief Sets the float value of the slider.
     *
     * Sets the float value of the slider, in order to update the spinbox updateValueSpinbox needs to be called after setting the value
     *
     * @param float tmpValue Must be between minValue and maxValue of the slider
     * @return void <DESCRIBE ME>
     */
    void setValue(float value);

    /**
     * \brief In contrast to setValue(float), initValue(float) does not emit signals. Also this is added due to lack of constructor.
     *
     * Sets the float value
     *
     * @param float tmpValue Must be between minValue and maxValue of the slider
     * @return void <DESCRIBE ME>
     */
    void initValue(float value) { value_ = value; }

    /**
     * \brief sets the maximum value of the spin box and slider
     *
     * <DESCRIBE THE METHOD>
     *
     * @param float max <DESCRIBE ME>
     * @return void <DESCRIBE ME>
     */
    void setMaxValue(float maxValue);
    /**
     * \brief sets the minimum value of the spin box and slider
     *
     * <DESCRIBE THE METHOD>
     *
     * @param float min <DESCRIBE ME>
     * @return void <DESCRIBE ME>
     */
    void setMinValue(float minValue);


    void setRange(float minValue, float maxValue);

    /**
     * \brief Sets the increment of the spinbox and slider
     *
     * <DESCRIBE THE METHOD>
     *
     * @param float increment <DESCRIBE ME>
     * @return void <DESCRIBE ME>
     */void setIncrement(float increment);



private slots:
    void updateFromSlider();
    void updateFromSpinBox();


signals:
    void valueChanged(float value);

private:
    float value_;
    float minValue_;
    float maxValue_;
    float increment_;
    static const int SLIDER_MAX = 10000;

    CustomDoubleSpinBoxQt* spinBox_;
    //QSlider* slider_;
    CustomSliderWidgetQt* slider_;

    void generateWidget();
    /**
     * \brief updates the value of the spin box from the slider value
     *
     * <DESCRIBE THE METHOD>
     *
     * @return void <DESCRIBE ME>
     */
    void updateSpinBox();
    /**
     * \brief updates the value of the slider from the spin box value
     *
     * <DESCRIBE THE METHOD>
     *
     * @return void <DESCRIBE ME>
     */

    void updateSlider();

    void setSpinBoxDecimals(float increment);
};

}//namespace

#endif // IVW_FLOATSLIDERQT_H