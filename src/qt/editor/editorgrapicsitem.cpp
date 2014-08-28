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

#include <inviwo/qt/editor/editorgrapicsitem.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <inviwo/core/ports/port.h>

namespace inviwo {

EditorGrapicsItem::EditorGrapicsItem() : QGraphicsRectItem() {}

EditorGrapicsItem::EditorGrapicsItem(QGraphicsItem* parent) : QGraphicsRectItem(parent) {}

EditorGrapicsItem::~EditorGrapicsItem() {}

QPoint EditorGrapicsItem::mapPosToSceen(QPointF inPos) const {
    if (scene() != NULL                                  // the focus item belongs to a scene
        && !scene()->views().isEmpty()                   // that scene is displayed in a view...
        && scene()->views().first() != NULL              // ... which is not null...
        && scene()->views().first()->viewport() != NULL  // ... and has a viewport
        ) {

        QPointF sceneP = mapToScene(inPos);
        QGraphicsView* v = scene()->views().first();
        QPoint viewP = v->mapFromScene(sceneP);
        return v->viewport()->mapToGlobal(viewP);
    } else {
        return QPoint(0,0);
    }

}

Port* EditorGrapicsItem::getInfoPort() const {
    return NULL;
}

const QPainterPath EditorGrapicsItem::makeRoundedBox(QRectF rect, float radius) {
    QPainterPath roundRectPath;
    roundRectPath.moveTo(rect.left(), rect.top() + radius);
    roundRectPath.lineTo(rect.left(), rect.bottom() - radius);
    roundRectPath.arcTo(rect.left(), rect.bottom() - (2 * radius), (2 * radius),
                        (2 * radius), 180.0, 90.0);
    roundRectPath.lineTo(rect.right() - radius, rect.bottom());
    roundRectPath.arcTo(rect.right() - (2 * radius), rect.bottom() - (2 * radius),
                        (2 * radius), (2 * radius), 270.0, 90.0);
    roundRectPath.lineTo(rect.right(), rect.top() + radius);
    roundRectPath.arcTo(rect.right() - (2 * radius), rect.top(), (2 * radius),
                        (2 * radius), 0.0, 90.0);
    roundRectPath.lineTo(rect.left() + radius, rect.top());
    roundRectPath.arcTo(rect.left(), rect.top(), (2 * radius), (2 * radius), 90.0,
                        90.0);

    return roundRectPath;
}


} // namespace

