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
 * Main file authors: Timo Ropinski, Erik Sundén
 *
 *********************************************************************************/

#include <inviwo/core/datastructures/histogram.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorview.h>
#include <inviwo/qt/widgets/properties/transferfunctionpropertydialog.h>
#include <QVarLengthArray>

namespace inviwo {

TransferFunctionEditorView::TransferFunctionEditorView(TransferFunctionProperty* tfProperty)
    : TransferFunctionObserver()
    , tfProperty_(tfProperty)
    , volumeInport_(tfProperty->getVolumeInport())
    , showHistogram_(tfProperty->getShowHistogram())
    , histogramTheadWorking_(false) {
    
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing, true);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    if (volumeInport_) {
        volumeInport_->onChange(this, &TransferFunctionEditorView::onTransferFunctionChange);
    }
}

void TransferFunctionEditorView::resizeEvent(QResizeEvent* event) {
    updateZoom();
    QGraphicsView::resizeEvent(event);
}

void TransferFunctionEditorView::drawForeground(QPainter* painter, const QRectF& rect) {
    if (tfProperty_->getMask().x > 0.0f) {
        int leftMaskBorder = tfProperty_->getMask().x*sceneRect().width();
        painter->fillRect(0, -5, leftMaskBorder, rect.height()+10, QColor(25,25,25,100));
        painter->drawLine(leftMaskBorder, 0, leftMaskBorder, rect.height());
    }

    if (tfProperty_->getMask().y < 1.0f) {
        int rightMaskBorder = tfProperty_->getMask().y*sceneRect().width();
        // add 10 to width to compensate scaling differences between scene and view
        painter->fillRect(rightMaskBorder, -5, sceneRect().width()-rightMaskBorder+10,
                          rect.height()+10, QColor(25,25,25,100));
        painter->drawLine(rightMaskBorder, 0, rightMaskBorder, rect.height());
    }

    QGraphicsView::drawForeground(painter, rect);
}

void TransferFunctionEditorView::onTransferFunctionChange() {
    volumeInport_ = tfProperty_->getVolumeInport();

    if (volumeInport_)
        volumeInport_->onChange(this, &TransferFunctionEditorView::onTransferFunctionChange);

    update();
}

void TransferFunctionEditorView::setShowHistogram(bool show) {
    showHistogram_ = show;
    update();
}

void TransferFunctionEditorView::histogramThreadFinished() {
    histogramTheadWorking_ = false;
    update();
}

const NormalizedHistogram* TransferFunctionEditorView::getNormalizedHistogram() {
    if (volumeInport_ && volumeInport_->hasData()) {
        const VolumeRAM* volumeRAM = volumeInport_->getData()->getRepresentation<VolumeRAM>();

        if (volumeRAM) {
            if (volumeRAM->hasNormalizedHistogram())
                return volumeRAM->getNormalizedHistogram();
            else if (!histogramTheadWorking_) {
                histogramTheadWorking_ = true;
                workerThread_ = new QThread();
                HistogramWorkerQt* worker = new HistogramWorkerQt(volumeRAM);
                worker->moveToThread(workerThread_);
                connect(workerThread_, SIGNAL(started()), worker, SLOT(process()));
                connect(worker, SIGNAL(finished()), workerThread_, SLOT(quit()));
                connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
                connect(workerThread_, SIGNAL(finished()), workerThread_, SLOT(deleteLater()));
                connect(workerThread_, SIGNAL(finished()), this, SLOT(histogramThreadFinished()));
                workerThread_->start();
            }

            return NULL;
        }
        else
            return NULL;
    }
    else
        return NULL;
}

void TransferFunctionEditorView::drawBackground(QPainter* painter, const QRectF& rect) {
    painter->fillRect(rect, QColor(89,89,89));
    painter->drawRect(rect);
    // overlay grid
    int gridSpacing = 25;
    QRectF sRect = sceneRect();
    qreal right = int(sRect.right()) - (int(sRect.right()) % gridSpacing);
    QVarLengthArray<QLineF, 100> lines;

    for (qreal x=sRect.left(); x<=right; x+=gridSpacing) {
        lines.append(QLineF(x, sRect.top(), x, sRect.bottom()));
    }

    QPen gridPen;
    gridPen.setColor(QColor(102,102,102));
    gridPen.setWidth(1.0);
    gridPen.setCosmetic(true);
    painter->setPen(gridPen);
    painter->drawLines(lines.data(), lines.size());

    // histogram
    if (showHistogram_) {
        const NormalizedHistogram* normHistogram = getNormalizedHistogram();

        if (normHistogram) {
            const std::vector<float>* normHistogramData = normHistogram->getData();
            QVarLengthArray<QLineF, 100> bars;

            for (size_t i=0; i<normHistogramData->size(); i++) {
                bars.append(QLineF(((float)i/(float)normHistogramData->size())*sRect.width(), 0.0,
                                   ((float)i/(float)normHistogramData->size())*sRect.width(), normHistogramData->at(i)*sRect.height()));
            }

            qreal lineWidth = sRect.width()/normHistogramData->size();
            painter->setPen(QPen(QColor(68,102,170,150), lineWidth));
            painter->drawLines(bars.data(), bars.size());
        }
    }
}

void TransferFunctionEditorView::updateZoom() {
    fitInView(tfProperty_->getZoomH().x * scene()->sceneRect().width(),
              tfProperty_->getZoomV().x * scene()->sceneRect().height(),
              (tfProperty_->getZoomH().y - tfProperty_->getZoomH().x) * scene()->sceneRect().width(),
              (tfProperty_->getZoomV().y - tfProperty_->getZoomV().x) * scene()->sceneRect().height(),
              Qt::IgnoreAspectRatio);
}

void TransferFunctionEditorView::zoomHorizontally(int zoomHMin, int zoomHMax) {
    // normalize zoom values, as sliders in TransferFunctionPropertyDialog
    // have the range [0...100]

    tfProperty_->setZoomH(static_cast<float>(zoomHMin) / 100.0f,
                          static_cast<float>(zoomHMax) / 100.0f);

    updateZoom();
}

void TransferFunctionEditorView::zoomVertically(int zoomVMin, int zoomVMax) {
    // normalize zoom values, as sliders in TransferFunctionPropertyDialog
    // have the range [0...100]
    // and flip/rescale values to compensate slider layout

    tfProperty_->setZoomV((100.0f - static_cast<float>(zoomVMax)) / 100.0f,
                          (100.0f - static_cast<float>(zoomVMin)) / 100.0f);

    updateZoom();
}


void HistogramWorkerQt::process() {
    volumeRAM_->getNormalizedHistogram();
    emit finished();
}

} // namespace inviwo