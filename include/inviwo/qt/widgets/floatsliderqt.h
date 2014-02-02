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