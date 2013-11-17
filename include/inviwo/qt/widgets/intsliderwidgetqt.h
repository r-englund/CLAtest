#ifndef IVW_INTSLIDERQT_H
#define IVW_INTSLIDERQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/customsliderwidgetqt.h>
#include <QSpinBox>
#include <QHBoxLayout>
#include <math.h>

namespace inviwo {

class IVW_QTWIDGETS_API IntSliderWidgetQt : public QWidget {

        Q_OBJECT;
public:

    IntSliderWidgetQt(int minValue_, int maxValue_, int increment_);

    virtual ~IntSliderWidgetQt();

 
    /** 
     * \brief Returns the integer value of the slider
     *
     * <DESCRIBE THE METHOD>
     * 
     * @return int <DESCRIBE ME>
     */
    int getValue();
 
    /** 
     * \brief Sets the value of the slider and spinbox
     *
     * <DESCRIBE THE METHOD>
     * 
     * @param int tmpValue <DESCRIBE ME>
     * @return void <DESCRIBE ME>
     */
    void setValue(int tmpValue);

    /** 
     * \brief sets the maximum value of the spin box and slider
     *
     * <DESCRIBE THE METHOD>
     * 
     * @param int max <DESCRIBE ME>
     * @return void <DESCRIBE ME>
     */
    void setMaxValue(int max);
    /** 
     * \brief sets the minimum value of the spin box and slider
     * <DESCRIBE THE METHOD>
     * 
     * @param int min <DESCRIBE ME>
     * @return void <DESCRIBE ME>
     */
    void setMinValue(int min);

    /** 
     * \brief Sets the minimum and maximum values of the slider and spin box
     *
     * <DESCRIBE THE METHOD>
     * 
     * @param int min <DESCRIBE ME>
     * @param int max <DESCRIBE ME>
     * @return void <DESCRIBE ME>
     */
    void setRange(int min,int max);


    void setIncrement(int increment);

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
     */void updateValueSlider();


    QSlider* getSlider();
    QSpinBox* getSpinBox();

    void updateSlider();

    private slots:
        void updateFromSlider();
        void updateFromSpinBox();


signals:
        void valueChanged(int value);
        


private:
    int value_;
    int sliderValue_;
    int maxValue_;
    int minValue_;
    int increment_;
    bool fromSlider_;
    QSpinBox* spinBox_;
    CustomSliderWidgetQt* slider_;
    void generateWidget();
    static const int SLIDER_MAX = 10000; 

};

}//namespace

#endif // IVW_INTSLIDERQT_H