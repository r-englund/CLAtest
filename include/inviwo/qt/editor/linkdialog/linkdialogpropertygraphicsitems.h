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
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

#ifndef  IVW_LINKDIALOG_PROPERTYGRAPHICSITEMS_H
#define  IVW_LINKDIALOG_PROPERTYGRAPHICSITEMS_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsView>
#include <QDialog>
#include <QGraphicsRectItem>
#include <QPushButton>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QPainterPath>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QComboBox>
#include <QStandardItemModel>
#include <QEventLoop>
#include <QCheckBox>

#include <inviwo/core/links/processorlink.h>
#include <inviwo/core/links/linkconditions.h>
#include <inviwo/core/network/processornetwork.h>
#include <inviwo/qt/widgets/labelgraphicsitem.h>
#include <inviwo/qt/editor/connectiongraphicsitem.h>
#include <inviwo/qt/widgets/inviwodockwidget.h>

#include <inviwo/qt/editor/linkdialog/linkdialoggraphicsitems.h>

namespace inviwo {

class DialogConnectionGraphicsItem;
class LinkDialogProcessorGraphicsItem;

class IVW_QTEDITOR_API LinkDialogPropertyGraphicsItem : public GraphicsItemData<Property> {

public:
    LinkDialogPropertyGraphicsItem(LinkDialogProcessorGraphicsItem*, Property*);
    ~LinkDialogPropertyGraphicsItem();

    void setProperty(Property* property);

    QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint = QSizeF()) const;

    //override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + LinkDialogPropertyGraphicsItemType };
    int type() const  {return Type; }

    QPointF getShortestBoundaryPointTo(LinkDialogPropertyGraphicsItem*);
    QPointF getShortestBoundaryPointTo(QPointF);

    void updatePositionBasedOnProcessor(bool isComposite=false);

    QPointF calculateArrowCenter(size_t curPort, bool computeRight) const;
    QRectF calculateArrowRect(size_t curPort, bool computeRight) const;
    QRectF calculateArrowRect(DialogConnectionGraphicsItem* cItem, bool computeRight=true) const;
    DialogConnectionGraphicsItem* getArrowConnectionAt(const QPointF pos) const;
    bool isArrowPointedRight(DialogConnectionGraphicsItem* cItem);

    //void addArrow() {arrowCount_++;}
    //int getArrowCount() {return arrowCount_;}
    //void removeArrow() {arrowCount_--;}

    void prepareGeometryChange() {QGraphicsItem::prepareGeometryChange();}

    void addConnectionGraphicsItem(DialogConnectionGraphicsItem*);
    size_t getConnectionGraphicsItemCount() const;
    void removeConnectionGraphicsItem(DialogConnectionGraphicsItem*);

protected:
    virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private:

    LabelGraphicsItem* classLabel_;
    LabelGraphicsItem* typeLabel_;
    LinkDialogProcessorGraphicsItem* processorGraphicsItem_;
    //int arrowCount_;
    std::vector<DialogConnectionGraphicsItem*> connectionItems_;
    bool isCompositeSubProperty_;
    bool isTopItem_;
    bool isBottomItem_;
};

} //namespace

#endif //IVW_LINKDIALOG_PROPERTYGRAPHICSITEMS_H
