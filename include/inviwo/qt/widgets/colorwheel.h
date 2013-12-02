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

#ifndef COLORWHEEL_H
#define COLORWHEEL_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QStyleOption>
#include <QtCore/qmath.h>
#include <QDebug>
namespace inviwo {

class ColorWheel : public QWidget
{
    Q_OBJECT
public:
    explicit ColorWheel(QWidget *parent = 0);

    virtual QSize sizeHint () const;
    virtual QSize minimumSizeHint () const;
    QColor color();

signals:
    void colorChange(const QColor &color);

    public slots:
        void setColor(const QColor &color);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *);
private:
    QSize initSize;
    QImage wheelImage;
    QImage squareImage;
    QPixmap wheel;
    bool mouseDown;
    QPoint lastPos;
    int margin;
    int wheelWidth;
    QRegion wheelRegion;
    QRegion squareRegion;
    QColor current;
    bool inWheel;
    bool inSquare;
    QColor posColor(const QPoint &point);
    void drawWheelImage(const QSize &newSize);
    void drawIndicator(const int &hue);
    void drawPicker(const QColor &color);
    void drawSquareImage(const int &hue);
    void composeWheel();
    double PI;
    private slots:
        void hueChanged(const int &hue);
        void svChanged(const QColor &newcolor);
};
} //Namespace
#endif // COLORWHEEL_H