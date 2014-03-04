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
 * Main file author: Timo Ropinski
 *
 *********************************************************************************/

#include <QMatrix>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/util/settings/linksettings.h>
#include <inviwo/core/util/settings/systemsettings.h>
#include <inviwo/qt/editor/networkeditorview.h>

namespace inviwo {

NetworkEditorView::NetworkEditorView(QWidget* parent) : QGraphicsView(parent),
    zoomLevel_(1)
{
    setNetworkEditor(NetworkEditor::getPtr());
    setRenderHint(QPainter::Antialiasing, true);
    setMouseTracking(true);
    setDragMode(QGraphicsView::RubberBandDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

NetworkEditorView::~NetworkEditorView() {
    QGraphicsView::setScene(0);
}


void NetworkEditorView::setNetworkEditor(NetworkEditor* networkEditor) {
    networkEditor_ = networkEditor;
    Property* displayLinkProperty = InviwoApplication::getPtr()->getSettingsByType<LinkSettings>()->getPropertyByIdentifier("displayLinks");

    if (displayLinkProperty) {
        displayLinkProperty->onChange(networkEditor_, &NetworkEditor::updateLinkGraphicsItems);
        networkEditor_->updateLinkGraphicsItems();
    }
    else
        LogWarn("Display Links property not found in settings");

    QGraphicsView::setScene(networkEditor);
}

NetworkEditor* NetworkEditorView::getNetworkEditor() const {
    return networkEditor_;
}

void NetworkEditorView::hideNetwork(bool hide){
    if(hide){
        if(scene()){
            scrollPos_.x = horizontalScrollBar()->value();
            scrollPos_.y = verticalScrollBar()->value();
            QGraphicsView::setScene(NULL);
        }
    }
    else{
        if(scene() != networkEditor_){
            QGraphicsView::setScene(networkEditor_);
            horizontalScrollBar()->setValue(scrollPos_.x);
            verticalScrollBar()->setValue(scrollPos_.y);
        }
    }
}

void NetworkEditorView::setZoomLevel(int zoomLevel) {
    if (zoomLevel < -10) zoomLevel = -10;
    else if (zoomLevel > 10) zoomLevel = 10;

    zoomLevel_ = zoomLevel;
    zoomValue_ = calculateScaleFor(zoomLevel);
    QMatrix matrix;
    matrix.scale(zoomValue_, zoomValue_);
    setMatrix(matrix);
}

void NetworkEditorView::mouseDoubleClickEvent(QMouseEvent* e) {
    QGraphicsView::mouseDoubleClickEvent(e);

    if (!e->isAccepted()) {
        fitInView(networkEditor_->itemsBoundingRect(), Qt::KeepAspectRatio);
        float scale = matrix().m11();
        zoomLevel_ = calculateZoomLevelFor(scale);
        e->accept();
    }
}

void NetworkEditorView::resizeEvent(QResizeEvent* e) {
    QGraphicsView::resizeEvent(e);
    setZoomLevel(zoomLevel_);
    e->accept();
}

void NetworkEditorView::wheelEvent(QWheelEvent* e) {
    int newZoomLevel = zoomLevel_;

    if (e->delta() > 0) newZoomLevel++;
    else newZoomLevel--;

    setZoomLevel(newZoomLevel);
    e->accept();
}

float NetworkEditorView::calculateScaleFor(int zoomLevel) const {
    float editorWidth = networkEditor_->width();
    float editorHeight = networkEditor_->height();
    float canvasSize = editorWidth > editorHeight ? editorWidth : editorHeight;
    float viewSize = editorWidth > editorHeight ?
                     float(viewport()->width()) :
                     float(viewport()->height());
    // at zoom level -10 canvasSize must become viewSize
    float zoomOutFactor = (viewSize*0.95f / canvasSize);
    // at zoom level +10 canvasSize must be 3 times its size
    float zoomInFactor = 3.0f / 10.0f;
    // calculate the zoom
    float zoom = 1.0f;

    if (zoomLevel < 0)
        zoom = 1.0f + float(zoomLevel) * (1.0f - zoomOutFactor) / 10.0f;
    else
        zoom = 1.0f + zoomInFactor * float(zoomLevel);

    return zoom;
}

int NetworkEditorView::calculateZoomLevelFor(float scale) const {
    float editorWidth = networkEditor_->width();
    float editorHeight = networkEditor_->height();
    float canvasSize = editorWidth > editorHeight ? editorWidth : editorHeight;
    float viewSize = editorWidth > editorHeight ?
                     float(viewport()->width()) :
                     float(viewport()->height());
    // at zoom level -10 canvasSize must become viewSize
    float zoomOutFactor = (viewSize*0.95f / canvasSize);
    // at zoom level +10 canvasSize must be 3 times its size
    float zoomInFactor = 3.0f / 10.0f;
    float zoomLevel = 1.0f;

    if (scale < 1)
        zoomLevel = (scale - 1.0f) * 10.0f / (1.0f - zoomOutFactor);
    else
        zoomLevel = (scale - 1.0f) / zoomInFactor;

    return int(zoomLevel);
}

} // namespace