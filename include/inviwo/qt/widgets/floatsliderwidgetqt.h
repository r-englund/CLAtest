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

    void setDecimals(int numberOfDecimals);

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

    QDoubleSpinBox* spinBox_;
    QSlider* slider_;

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
};

}//namespace

#endif // IVW_FLOATSLIDERQT_H