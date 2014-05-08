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
 * Main file authors: Erik Sundén, Viktor Axelsson, Timo Ropinski
 *
 *********************************************************************************/

#ifndef IVW_TRANSFERFUNCTIONEDITORVIEW_H
#define IVW_TRANSFERFUNCTIONEDITORVIEW_H

#include <inviwo/core/properties/transferfunctionproperty.h>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditor.h>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/core/datastructures/volume/volumeram.h>
#include <QtEvents>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QThread>

namespace inviwo {
class IVW_QTWIDGETS_API TransferFunctionEditorView : public QGraphicsView, public TransferFunctionObserver {

    Q_OBJECT

public:
    TransferFunctionEditorView(TransferFunctionProperty* tfProperty);

    void setMask(float maskMin, float maskMax) {
        if (maskMax<maskMin) {
            maskMax=maskMin;
        }
        maskHorizontal_ = vec2(maskMin, maskMax);
        this->viewport()->update();
    }
    
    void onVolumeInportInvalid();
    void onTransferFunctionChange();
    void onControlPointChanged(const TransferFunctionDataPoint* p);
    void setShowHistogram(bool);

signals:
    void resized();

public slots:
    void histogramThreadFinished();
    //void zoomHorizontally(int zoomHMin, int zoomHMax);
    //void zoomVertically(int zoomVMin, int zoomVMax);

    void updateZoom();
protected:
    const NormalizedHistogram* getNormalizedHistogram();

    void resizeEvent(QResizeEvent* event);

    void drawForeground(QPainter* painter, const QRectF& rect);
    void drawBackground(QPainter* painter, const QRectF& rect);

    void updateHistogram();
    void onVolumeInportChange();
private:
    TransferFunctionProperty* tfProperty_;
    VolumeInport* volumeInport_;
    bool showHistogram_;

    double barWidth_;
    std::vector<QLineF> histogramBars_;

    bool histogramTheadWorking_;
    QThread* workerThread_;
    
    bool invalidatedHistogram_;
    vec2 maskHorizontal_;
};

class IVW_QTWIDGETS_API HistogramWorkerQt : public QObject {
    Q_OBJECT
public:
    HistogramWorkerQt(const VolumeRAM* volumeRAM, std::size_t numBins=2048u) : volumeRAM_(volumeRAM), numBins_(numBins) {}
    ~HistogramWorkerQt() { volumeRAM_ = NULL; };

public slots:
    void process();

signals:
    void finished();

private:
    const VolumeRAM* volumeRAM_;
    const std::size_t numBins_;
};

} // namespace

#endif // IVW_TRANSFERFUNCTIONEDITORVIEW_H