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
 * Main file authors: Alexander Johansson, Daniel Jönsson
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/lightpositionwidgetqt.h>

namespace inviwo {


LightPositionWidgetQt::LightPositionWidgetQt() : mouseDown_(false) {
    generateWidget();
}

LightPositionWidgetQt::~LightPositionWidgetQt() {
    delete painter_;
    delete gradient_;
    delete gradientPixmap_;
}

void inviwo::LightPositionWidgetQt::generateWidget() {
    gradientPixmap_ = new QPixmap(100,100);
    gradientPixmap_->fill(Qt::transparent);
    this->setFixedWidth(100);
    this->setFixedHeight(100);
    gradient_ = new QRadialGradient(50, 50, 50, 50, 50);
    gradient_->setColorAt(0, QColor::fromRgbF(1, 1, 1, 1));
    gradient_->setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
    radius_ = gradient_->radius();
    painter_ = new QPainter(gradientPixmap_);
    painter_->fillRect(0, 0, 100, 100, *gradient_);
    this->setPixmap(*gradientPixmap_);
}


void LightPositionWidgetQt::mousePressEvent(QMouseEvent* event) {
    mouseDown_ = true;
}

void LightPositionWidgetQt::mouseMoveEvent(QMouseEvent* event) {
    if (!mouseDown_) return;

    setNewPosition(event);
}

void LightPositionWidgetQt::mouseReleaseEvent(QMouseEvent* event) {
    setNewPosition(event);
}

void LightPositionWidgetQt::setNewPosition(QMouseEvent* event) {
    const QPoint p = event->pos();
    QPoint center(gradientPixmap_->width()/2, gradientPixmap_->height()/2);
    float x = p.x()-center.x();
    float y = p.y()-center.y();
    float gradientSpaceRadius = sqrt(x*x+y*y);

    // Check if user clicked close to, or outside of radius
    if (gradientSpaceRadius+3.f > gradient_->radius()) {
        // User clicked outside of radius so we need to normalize x,y coordinate
        // Add a small number to avoid gradient on the border
        float normFactor = gradient_->radius()/(3.f+gradientSpaceRadius);
        x *= normFactor;
        y *= normFactor;
    }

    float z = sqrt(gradient_->radius()*gradient_->radius()-x*x-y*y);
    theta_ = acos(z/gradient_->radius());
    phi_ = atan2(y, x);
    // Spherical to cartesian coordinates
    float x1=sin(theta_)*cos(phi_)*gradient_->radius()+center.x();
    float y1=sin(theta_)*sin(phi_)*gradient_->radius()+center.y();
    QPointF newPoint(x1,y1);
    gradient_->setFocalPoint(newPoint);
    gradientPixmap_->fill(Qt::transparent);
    painter_->fillRect(0, 0, 100, 100, *gradient_);
    this->setPixmap(*gradientPixmap_);
    emit positionChanged();
}

void LightPositionWidgetQt::setPosition(const vec3& p) {
    radius_ = glm::length(p);

    if (radius_==0)
        return;

    QPointF center(gradientPixmap_->width()/2, gradientPixmap_->height()/2);
    theta_ = acos(p.z/radius_);
    phi_ = atan2(p.y, p.x);
    float x1=sin(theta_)*cos(phi_)*gradient_->radius()+center.x();
    float y1=sin(theta_)*sin(phi_)*gradient_->radius()+center.y();
    float gradientSpaceRadius = sqrt(pow(x1-center.x(),2)+pow(y1-center.y(),2));

    // Check if user clicked close to, or outside of radius
    if (gradientSpaceRadius+3.f > gradient_->radius()) {
        float normFactor = gradient_->radius()/(3.f+gradientSpaceRadius);
        x1 *= normFactor;
        y1 *= normFactor;
    }

    QPointF newPoint(x1,y1);
    gradient_->setFocalPoint(newPoint);
    gradientPixmap_->fill(Qt::transparent);
    painter_->fillRect(0, 0, 100, 100, *gradient_);
    this->setPixmap(*gradientPixmap_);
}

}//namespace