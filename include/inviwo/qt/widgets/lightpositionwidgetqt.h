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
#include <QPainter>
#include <QBrush>
#include <QMouseEvent>
#include <QPoint>
#include <QtCore/qmath.h>

#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_QTWIDGETS_API LightPositionWidgetQt : public QLabel {
    Q_OBJECT

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
public:

    LightPositionWidgetQt();
    void setPosition(const vec3& positionVector);
    vec3 getPosition() const { return vec3(getX(), getY(), getZ()); }
    virtual ~LightPositionWidgetQt();


    float getX() const { return radius_*sin(theta_)*cos(phi_); };
    float getY() const { return radius_*sin(theta_)*sin(phi_); };
    float getZ() const { return radius_*cos(theta_); };


signals:
    void positionChanged();


private:
    QPixmap* gradientPixmap_;
    QRadialGradient* gradient_;
    QPainter* painter_;
    bool mouseDown_;
    void generateWidget();
    void setNewPosition(QMouseEvent* event);
    float radius_;
    float theta_;
    float phi_;

};

}//namespace

#endif // IVW_INTSLIDERQT_H