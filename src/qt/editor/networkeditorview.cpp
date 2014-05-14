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
#include <inviwo/core/network/processornetwork.h>

namespace inviwo {

NetworkEditorView::NetworkEditorView(QWidget* parent)
    : QGraphicsView(parent)
    , NetworkEditorObserver()
    , zoom_(1.0f) {
    setNetworkEditor(NetworkEditor::getPtr());
    setRenderHint(QPainter::Antialiasing, true);
    setMouseTracking(true);
    setDragMode(QGraphicsView::RubberBandDrag);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

NetworkEditorView::~NetworkEditorView() { QGraphicsView::setScene(NULL); }

void NetworkEditorView::setNetworkEditor(NetworkEditor* networkEditor) {
    networkEditor_ = networkEditor;
    Property* displayLinkProperty =
        InviwoApplication::getPtr()->getSettingsByType<LinkSettings>()->getPropertyByIdentifier(
            "displayLinks");

    if (displayLinkProperty) {
        displayLinkProperty->onChange(networkEditor_, &NetworkEditor::updateLinkGraphicsItems);
        networkEditor_->updateLinkGraphicsItems();
    } else
        LogWarn("Display Links property not found in settings");

    NetworkEditorObserver::addObservation(networkEditor_);	

    QGraphicsView::setScene(networkEditor);
}

NetworkEditor* NetworkEditorView::getNetworkEditor() const { return networkEditor_; }

void NetworkEditorView::hideNetwork(bool hide) {
    if (hide) {
        if (scene()) {
            scrollPos_.x = horizontalScrollBar()->value();
            scrollPos_.y = verticalScrollBar()->value();
            QGraphicsView::setScene(NULL);
        }
    } else {
        if (scene() != networkEditor_) {
            QGraphicsView::setScene(networkEditor_);
            horizontalScrollBar()->setValue(scrollPos_.x);
            verticalScrollBar()->setValue(scrollPos_.y);
        }
    }
}

void NetworkEditorView::setZoom(const float& zoom) {
    if (zoom < 0.2)
        zoom_ = 0.2;
    else if (zoom > 5.0)
        zoom_ = 5.0;
    else
        zoom_ = zoom;

    QTransform matrix;
    matrix.scale(zoom_, zoom_);
    setTransform(matrix);
}

void NetworkEditorView::mouseDoubleClickEvent(QMouseEvent* e) {
    QGraphicsView::mouseDoubleClickEvent(e);

    if (!e->isAccepted()) {
        fitNetwork();
        e->accept();
    }
}

void NetworkEditorView::resizeEvent(QResizeEvent* e) {
    QGraphicsView::resizeEvent(e);
    fitNetwork();
    e->accept();
}

void NetworkEditorView::fitNetwork() {
    const ProcessorNetwork* network = networkEditor_->getProcessorNetwork();
    if (network) {
        if (network->getProcessors().size() > 0) {
            QRectF br = networkEditor_->itemsBoundingRect().adjusted(-50, -50, 50, 50);
            fitInView(br, Qt::KeepAspectRatio);
            zoom_ = transform().m11();
        }
    }
}

void NetworkEditorView::wheelEvent(QWheelEvent* e) {
    QPoint numPixels = e->pixelDelta();
    QPoint numDegrees = e->angleDelta() / 8;

    if (e->modifiers() == Qt::ControlModifier) {

        if (!numPixels.isNull()) {
            setZoom(zoom_ + static_cast<float>(numPixels.y()) / 50.0);
        } else if (!numDegrees.isNull()) {
            // This needs tuning for windows...
            setZoom(zoom_ + static_cast<float>(numPixels.y()) / 50.0);
        }
    } else {
        QGraphicsView::wheelEvent(e);
    }
    e->accept();
}

void NetworkEditorView::onNetworkEditorFileChanged(const std::string& newFilename) {
    fitNetwork();
}
void NetworkEditorView::onModifiedStatusChanged(const bool& newStatus) {}

}  // namespace