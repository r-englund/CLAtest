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

#include <inviwo/qt/widgets/lightpositionwidgetqt.h>

namespace inviwo {
    inviwo::LightPositionWidgetQt::LightPositionWidgetQt() : mouseDown_(false){
    generateWidget();

}

void inviwo::LightPositionWidgetQt::generateWidget() {

    gradientPixmap_ = new QPixmap(100,100);
    gradientPixmap_->fill(Qt::transparent);

    this->setStyleSheet("border: 2px solid");
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

inviwo::LightPositionWidgetQt::~LightPositionWidgetQt(){}


void LightPositionWidgetQt::mousePressEvent( QMouseEvent* event ){
    mouseDown_ = true;
}

void LightPositionWidgetQt::mouseMoveEvent( QMouseEvent * event ){
    if (!mouseDown_) return;    

    setNewPosition(event);
}

void LightPositionWidgetQt::mouseReleaseEvent( QMouseEvent *event ){
    setNewPosition(event);
}

void LightPositionWidgetQt::setNewPosition( QMouseEvent * event ){
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

void LightPositionWidgetQt::setPosition( const vec3& p ){
    radius_ = glm::length(p);
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