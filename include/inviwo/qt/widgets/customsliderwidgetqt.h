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

#ifndef IVW_CUSTOMSLIDERWIDGETQT_H
#define IVW_CUSTOMSLIDERWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QSlider>
#include <QMouseEvent>
#include <QtCore/qmath.h>

namespace inviwo {

class IVW_QTWIDGETS_API CustomSliderWidgetQt : public QSlider{
    Q_OBJECT

protected:
    void mousePressEvent ( QMouseEvent * event )
    {
        if (event->button() == Qt::LeftButton)
        {
            if (orientation() == Qt::Vertical){
                //FIX ME
                //Hardcoded value taken from the stylesheet for Qslider
                double halfHandleHeight = 6.5;     
                int adaptedPosY = event->y();
                if ( adaptedPosY < halfHandleHeight )
                    adaptedPosY = halfHandleHeight;
                if ( adaptedPosY > height() - halfHandleHeight )
                    adaptedPosY = height() - halfHandleHeight;

                double newWidth = (height() - halfHandleHeight) - halfHandleHeight;
                double normalizedPosition = (adaptedPosY - halfHandleHeight)  / newWidth ;

                int newVal = minimum() + ((maximum()-minimum()) * normalizedPosition);
                setValue(newVal);
            }
            else {
                //FIX ME
                //Hardcoded value taken from the stylesheet for Qslider
                double halfHandleWidth = 6.5;
                int adaptedPosX = event->x();
                if ( adaptedPosX < halfHandleWidth )
                    adaptedPosX = halfHandleWidth;
                if ( adaptedPosX > width() - halfHandleWidth )
                    adaptedPosX = width() - halfHandleWidth;

                double newWidth = (width() - halfHandleWidth) - halfHandleWidth;
                double normalizedPosition = (adaptedPosX - halfHandleWidth)  / newWidth ;

                int newVal = minimum() + ((maximum()-minimum()) * normalizedPosition);
                setValue(newVal);
            }

            event->accept();
        }
        QSlider::mousePressEvent(event);
    }
};

}

#endif //IVW_CUSTOMSLIDERWIDGETQT_H