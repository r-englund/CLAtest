 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013 Inviwo Foundation
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

#ifndef IVW_FLOATSLIDERQT_H
#define IVW_FLOATSLIDERQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QSlider>

namespace inviwo {

class IVW_QTWIDGETS_API FloatSliderQt : public QSlider {

public:
    /**
     * \brief <VERY BRIEFLY DESCRIBE THE METHOD>
     *
     * <DESCRIBE THE METHOD>
     *
     * @param float minValue_ <DESCRIBE ME>
     * @param float maxValue_ <DESCRIBE ME>
     * @param float stepSize_ <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */
    FloatSliderQt(float minValue_, float maxValue_);

    virtual ~FloatSliderQt();

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
     * <DESCRIBE THE METHOD>
     *
     * @param float tmpValue Must be between minValue and maxValue of the slider
     * @return void <DESCRIBE ME>
     */
    void setValue(float tmpValue);
    void setMaxValue(float tmpValue);
    void setMinValue(float tmpValue);
    void setRange(float min,float max);


    //void setOrientation(Qt::Orientation orientation_);

private:
    float sliderValue_;
    float maxValue_;
    float minValue_;
};

}//namespace

#endif // IVW_FLOATSLIDERQT_H