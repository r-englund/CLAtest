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
 * Main file authors: Timo Ropinski, Erik Sundén
 *
 *********************************************************************************/

#ifndef IVW_PROCESSORGRAPHICSITEM_H
#define IVW_PROCESSORGRAPHICSITEM_H

#include <inviwo/core/processors/processor.h>
#include <inviwo/core/processors/progressbar.h>
#include <inviwo/qt/widgets/labelgraphicsitem.h>
#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsRectItem>
#include <QTime>

namespace inviwo {

// TODO: Move this to common header later on (eg., BaseGraphicsItem.h )
static const qreal DRAGING_ITEM_DEPTH = 4.0f;
static const qreal PROCESSORGRAPHICSITEM_DEPTH = 2.0f;
static const qreal SELECTED_PROCESSORGRAPHICSITEM_DEPTH = 3.0f;
static const qreal CONNECTIONGRAPHICSITEM_DEPTH = 1.0f;
static const qreal LINKGRAPHICSITEM_DEPTH = 1.0f;

IVW_QTEDITOR_API enum InviwoUserGraphicsItemType {
    ProcessorGraphicsType = 1,
    ConnectionGraphicsType = 2,
    LinkGraphicsType = 3
};

class IVW_QTEDITOR_API ProcessorGraphicsItem : public QGraphicsRectItem,
                                               public ProcessorObserver,
                                               public LabelGraphicsItemObserver,
                                               public ProgressBarObserver {

public:
    ProcessorGraphicsItem();
    ~ProcessorGraphicsItem();

    Processor* getProcessor() const { return processor_; }
    std::string getIdentifier() const { return processor_->getIdentifier(); }
    void setProcessor(Processor* processor);
    void editProcessorName();
    QRectF calculatePortRect(Inport* port) const;
    QRectF calculatePortRect(Outport* port) const;
    QRectF calculateInportRect(size_t curPort) const;
    QRectF calculateOutportRect(size_t curPort) const;
    Port* getSelectedPort(const QPointF pos) const;
    void updateMetaData();
    QPointF getShortestBoundaryPointTo(ProcessorGraphicsItem* processorGraphicsItem);
    QPointF getShortestBoundaryPointTo(QPointF inPos);

    void onLabelGraphicsItemChange();
    bool isEditingProcessorName();

    // override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + ProcessorGraphicsType };
    int type() const { return Type; }

protected:
    void setIdentifier(QString text);
    void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    void paintStatusIndicator(QPainter* p, QPointF pos, bool status, QColor baseColor);
    void paintProgressBar(QPainter* p, float progress);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

    virtual void progressChanged();
    virtual void progressBarVisibilityChanged();
    void updateViews();

private:
    Processor* processor_;
    LabelGraphicsItem* nameLabel_;
    LabelGraphicsItem* classLabel_;

    QTime progressBarTimer_;
};

}  // namespace

#endif  // IVW_PROCESSORGRAPHICSITEM_H