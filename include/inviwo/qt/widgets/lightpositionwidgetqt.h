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

#ifndef IVW_LIGHTPOSITIONWIDGETQT_H
#define IVW_LIGHTPOSITIONWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QLabel>
#include <QRadialGradient>
#include <QPixmap>
#include <Qpainter>
#include <QBrush>
#include <QMouseEvent>
#include <QPoint>
#include <QtCore/qmath.h>

namespace inviwo {

class IVW_QTWIDGETS_API LightPositionWidgetQt : public QLabel {
        Q_OBJECT

protected:
    void mousePressEvent ( QMouseEvent * event );
    void mouseMoveEvent ( QMouseEvent * event );
    void mouseReleaseEvent(QMouseEvent *event);
public:

    LightPositionWidgetQt();
    void setPosition( ivec3 positionVector);
    virtual ~LightPositionWidgetQt();


    /** 
     * \brief Returns the integer value of the slider
     *
     * <DESCRIBE THE METHOD>
     * 
     * @return int <DESCRIBE ME>
     */
    int getValue();
    int getX(){return x0_;};
    int getY(){return y0_;};
    int getR(){return R_;};
 

signals:
        void positionChanged();
        

private:
    QPixmap* gradientPixmap_;
    QRadialGradient* gradient_;
    QPainter* painter_;
    bool mouseDown_;
    void generateWidget();
    void setNewPosition(QMouseEvent * event );
    int R_;
    int x0_;
    int y0_;

};

}//namespace

#endif // IVW_INTSLIDERQT_H