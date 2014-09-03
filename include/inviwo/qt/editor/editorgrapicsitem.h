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
 * Main file authors: Peter Steneteg
 *
 *********************************************************************************/

#ifndef IVW_EDITORGRAPICSITEM_H
#define IVW_EDITORGRAPICSITEM_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneHelpEvent>

namespace inviwo {

IVW_QTEDITOR_API enum InviwoUserGraphicsItemType {
    ProcessorGraphicsType = 1,
    CurveGraphicsType,
    ConnectionDragGraphicsType,
    ConnectionGraphicsType,
    LinkGraphicsType,
    LinkConnectionDragGraphicsType, 
    LinkConnectionGraphicsType, 
    ProcessorProgressGraphicsType,
    ProcessorStatusGraphicsType,
    ProcessorLinkGraphicsType,
    ProcessorInportGraphicsType,
    ProcessorOutportGraphicsType
};

// Z value for various graphics items.
static const qreal DRAGING_ITEM_DEPTH = 4.0f;
static const qreal PROCESSORGRAPHICSITEM_DEPTH = 2.0f;
static const qreal SELECTED_PROCESSORGRAPHICSITEM_DEPTH = 3.0f;
static const qreal CONNECTIONGRAPHICSITEM_DEPTH = 1.0f;
static const qreal LINKGRAPHICSITEM_DEPTH = 1.0f;

class Port;

class IVW_QTEDITOR_API EditorGrapicsItem : public QGraphicsRectItem {
public:
    EditorGrapicsItem();
    EditorGrapicsItem(QGraphicsItem* parent);
    virtual ~EditorGrapicsItem();
    QPoint mapPosToSceen(QPointF pos) const;

    static const QPainterPath makeRoundedBox(QRectF rect, float radius );

    virtual void showToolTip(QGraphicsSceneHelpEvent* event);
    void showPortInfo(QGraphicsSceneHelpEvent* e, Port* port) const;

protected:
    void showToolTipHelper(QGraphicsSceneHelpEvent* event, QString string) const;
};

}  // namespace

#endif  // IVW_EDITORGRAPICSITEM_H
