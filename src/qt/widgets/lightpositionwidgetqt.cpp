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
    R_ = gradient_->radius();

    painter_ = new QPainter(gradientPixmap_);
    QBrush* brush = new QBrush(*gradient_);

    painter_->fillRect(0, 0, 100, 100, *gradient_);
    this->setPixmap(*gradientPixmap_);
}

inviwo::LightPositionWidgetQt::~LightPositionWidgetQt(){}

int inviwo::LightPositionWidgetQt::getValue() {
    return 0;
}


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

    double x0 = p.x()-center.x();
    double y0 = p.y()-center.y();
    x0_ = x0;
    y0_ = y0;
    int x1=p.x();
    int y1=p.y();
    double vNorm = qSqrt(qPow(x0,2)+qPow(y0,2));

    if (vNorm >= R_) {
        x1 = R_*(x0_/vNorm);
        y1 = R_*(y0_/vNorm);
        x0_ = x1;
        y0_ = y1;
        x1 += center.x();
        y1 += center.y();
    }

    QPoint newPoint(x1,y1);
    gradient_->setFocalPoint(newPoint);
    gradientPixmap_->fill(Qt::transparent);
    painter_->fillRect(0, 0, 100, 100, *gradient_);
    this->setPixmap(*gradientPixmap_);
    emit positionChanged();
}

void LightPositionWidgetQt::setPosition( ivec3 positionVector ){
    QPoint center(gradientPixmap_->width()/2, gradientPixmap_->height()/2);

    QPoint newPoint(positionVector.x +  center.x(),positionVector.y +  center.y());
    gradient_->setFocalPoint(newPoint);
    gradientPixmap_->fill(Qt::transparent);
    painter_->fillRect(0, 0, 100, 100, *gradient_);
    this->setPixmap(*gradientPixmap_);
}

}//namespace