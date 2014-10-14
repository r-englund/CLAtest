/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Contact: Sathish Kottravel
 *
 *********************************************************************************/

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
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
public:

    LightPositionWidgetQt();
    void setPosition(const vec3& positionVector);
    vec3 getPosition() const { return vec3(getX(), getY(), getZ()); }
    virtual ~LightPositionWidgetQt();


    float getX() const { return sin(theta_)*cos(phi_); };
    float getY() const { return sin(theta_)*sin(phi_); };
    float getZ() const { return cos(theta_); };

    void setRadius(float radius) { radius_ = radius; positionChanged(); }
    float getRadius() const { return radius_; }

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