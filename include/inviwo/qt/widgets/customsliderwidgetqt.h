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

#ifndef IVW_CUSTOMSLIDERWIDGETQT_H
#define IVW_CUSTOMSLIDERWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QSlider>
#include <QMouseEvent>
#include <QtCore/qmath.h>

namespace inviwo {

class IVW_QTWIDGETS_API CustomSliderWidgetQt : public QSlider {
    Q_OBJECT

protected:
    void mousePressEvent(QMouseEvent* event) {
        if (event->button() == Qt::LeftButton) {
            if (orientation() == Qt::Vertical) {
                //FIX ME
                //Hardcoded value taken from the stylesheet for Qslider
                double halfHandleHeight = 6.5;
                int adaptedPosY = event->y();

                if (adaptedPosY < halfHandleHeight)
                    adaptedPosY = halfHandleHeight;

                if (adaptedPosY > height() - halfHandleHeight)
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

                if (adaptedPosX < halfHandleWidth)
                    adaptedPosX = halfHandleWidth;

                if (adaptedPosX > width() - halfHandleWidth)
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