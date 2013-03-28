#ifndef IVW_FLOATSLIDERQT_H
#define IVW_FLOATSLIDERQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <math.h>

namespace inviwo {

class IVW_QTWIDGETS_API FloatSliderWidgetQt : public QWidget {

        Q_OBJECT;
public:
    /** 
     * \brief <VERY BRIEFLY DESCRIBE THE METHOD>
     *
     * <DESCRIBE THE METHOD>
     * 
     * @param float minValue_ <DESCRIBE ME>
     * @param float maxValue_ <DESCRIBE ME>
     * @param float increment_ <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */
    FloatSliderWidgetQt(float minValue_, float maxValue_, float increment_);

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
    void setValue(float tmpValue);
    /** 
     * \brief sets the maximum value of the spin box and slider
     *
     * <DESCRIBE THE METHOD>
     * 
     * @param float max <DESCRIBE ME>
     * @return void <DESCRIBE ME>
     */
    void setMaxValue(float max);
    /** 
     * \brief sets the minimum value of the spin box and slider
     *
     * <DESCRIBE THE METHOD>
     * 
     * @param float min <DESCRIBE ME>
     * @return void <DESCRIBE ME>
     */
    void setMinValue(float min);

    /** 
     * \brief sets the range of the slider and spin box
     *
     * <DESCRIBE THE METHOD>
     * 
     * @param float min <DESCRIBE ME>
     * @param float max <DESCRIBE ME>
     * @return void <DESCRIBE ME>
     */
    void setRange(float min,float max);

    /** 
     * \brief Sets the increment of the spinbox and slider
     *
     * <DESCRIBE THE METHOD>
     * 
     * @param float increment <DESCRIBE ME>
     * @return void <DESCRIBE ME>
     */void setIncrement(float increment);

    /** 
     * \brief updates the value of the spin box from the slider value
     *
     * <DESCRIBE THE METHOD>
     * 
     * @return void <DESCRIBE ME>
     */
    void updateValueSpinBox();
    /** 
     * \brief updates the value of the slider from the spin box value
     *
     * <DESCRIBE THE METHOD>
     * 
     * @return void <DESCRIBE ME>
     */

    void updateValueSlider();
    QDoubleSpinBox* getSpinBox();
    QSlider* getSlider();

private:
    float sliderValue_;
    float maxValue_;
    float minValue_;
    float increment_;
    QDoubleSpinBox* spinBox_;
    QSlider* slider_;
    void generateWidget();

};

}//namespace

#endif // IVW_FLOATSLIDERQT_H