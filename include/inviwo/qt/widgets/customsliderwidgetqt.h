#ifndef IVW_CUSTOMSLIDERWIDGETQT_H
#define IVW_CUSTOMSLIDERWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QSlider>
#include <QMouseEvent>

namespace inviwo {

class IVW_QTWIDGETS_API CustomSliderWidgetQt : public QSlider
{

protected:
    void mousePressEvent ( QMouseEvent * event )
    {
        if (event->button() == Qt::LeftButton)
        {
            if (orientation() == Qt::Vertical)
                setValue(minimum() + ((this->maximum()-this->minimum()) * (this->height()-event->y())) / this->height() ) ;
            else
                setValue(this->minimum() + ((this->maximum()-this->minimum()) * event->x()) / this->width() ) ;

            event->accept();
        }
        QSlider::mousePressEvent(event);
    }
};

}//namespace

#endif //IVW_CUSTOMSLIDERWIDGETQT_H