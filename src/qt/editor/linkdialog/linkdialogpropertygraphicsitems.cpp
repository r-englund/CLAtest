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

#include <QGraphicsDropShadowEffect>
#include <QVector2D>

#include <inviwo/qt/editor/linkdialog/linkdialogpropertygraphicsitems.h>
#include <inviwo/qt/editor/linkdialog/linkdialogprocessorgraphicsitems.h>
#include <inviwo/qt/editor/linkdialog/linkdialogcurvegraphicsitems.h>

namespace inviwo {

LinkDialogPropertyGraphicsItem::LinkDialogPropertyGraphicsItem(LinkDialogProcessorGraphicsItem* processor,
        Property* prop) : GraphicsItemData<Property>()
        , isCompositeSubProperty_(false)
        , isTopItem_(true)
        , isBottomItem_(true) {
    setZValue(LINKDIALOG_PROCESSOR_GRAPHICSITEM_DEPTH);
    //setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable | ItemSendsGeometryChanges);
    setRect(-propertyItemWidth/2, -propertyItemHeight/2, propertyItemWidth, propertyItemHeight);
    QGraphicsDropShadowEffect* processorShadowEffect = new QGraphicsDropShadowEffect();
    processorShadowEffect->setOffset(3.0);
    processorShadowEffect->setBlurRadius(3.0);
    setGraphicsEffect(processorShadowEffect);
    classLabel_ = new LabelGraphicsItem(this);
    classLabel_->setPos(-propertyItemWidth/2.0+propertyLabelHeight/2.0, -propertyItemHeight/2.0+propertyLabelHeight);
    classLabel_->setDefaultTextColor(Qt::black);
    classLabel_->setFont(QFont("Segoe", propertyLabelHeight, QFont::Black, false));
    classLabel_->setCrop(9, 8);
    typeLabel_ = new LabelGraphicsItem(this);
    typeLabel_->setPos(-propertyItemWidth/2.0+propertyLabelHeight/2.0, -propertyItemHeight/2.0+propertyLabelHeight*2.5);
    typeLabel_->setDefaultTextColor(Qt::black);
    typeLabel_->setFont(QFont("Segoe", processorLabelHeight, QFont::Normal, true));
    typeLabel_->setCrop(9, 8);
    processorGraphicsItem_ = processor;
    setProperty(prop);
    updatePositionBasedOnProcessor();
}

void LinkDialogPropertyGraphicsItem::updatePositionBasedOnProcessor(bool isComposite) {
    if (!processorGraphicsItem_) return;

    Processor* processor = processorGraphicsItem_->getProcessor();
    int ind = 0;
    int subPropInd = 0;
    Property* prop = getGraphicsItemData();

    if (prop) {
        std::vector<Property*> properties = processor->getProperties();

        for (size_t i=0; i<properties.size(); i++) {
            if (prop == properties[i])
                break;

            bool found = false;
            CompositeProperty* compPorp = IS_COMPOSITE_PROPERTY( properties[i]);
            if (compPorp && isComposite) {
                std::vector<Property*> subProperties = compPorp->getProperties();
                
                for (size_t j=0; j<subProperties.size(); j++) { 
                    if (prop == subProperties[j]) {
                        //LogWarn("Found property in composite property")
                        isCompositeSubProperty_ = true;
                        found = true;
                        break;
                    }
                    ind++;
                    subPropInd++;
                }

                if (subPropInd == 0 && found)  {
                    isTopItem_ = true;  
                    isBottomItem_ = false;
                }
                else if (subPropInd >= static_cast<int>(subProperties.size())-1  && found) {
                    isBottomItem_ = true; 
                    isTopItem_ = false;
                }
                else if (found) {
                    isBottomItem_ = false; 
                    isTopItem_ = false;
                }

                if (found) break;
                continue;
                //ind+=subPropInd;
            }

            if (found) break;
            ind++;
        }
    }

    QPointF tr;
    QPointF br;
    tr = processorGraphicsItem_->rect().topRight();
    br = processorGraphicsItem_->rect().bottomRight();
    QPointF processorMappedDim = processorGraphicsItem_->mapToParent(tr) - processorGraphicsItem_->mapToParent(br);
    tr = rect().topRight();
    br = rect().bottomRight();
    QPointF propertyMappedDim = mapToParent(tr) -  mapToParent(br);
    qreal initialOffset = fabs(processorMappedDim.y()/2.0) + fabs(propertyMappedDim.y()/2.0);
    QPointF p = processorGraphicsItem_->pos();
    setPos(QPointF(p.x(), p.y()+ initialOffset + (ind*fabs(propertyMappedDim.y()))));
}

LinkDialogPropertyGraphicsItem::~LinkDialogPropertyGraphicsItem() {}

QSizeF LinkDialogPropertyGraphicsItem::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
    switch (which) {
        case Qt::MinimumSize:
        case Qt::MaximumSize:
        case Qt::PreferredSize:
            return rect().size() + QSize(12, 12);

        default:
            break;
    }

    return constraint;
}

void LinkDialogPropertyGraphicsItem::addConnectionGraphicsItem(DialogConnectionGraphicsItem* cItem) {
    connectionItems_.push_back(cItem);
}

size_t LinkDialogPropertyGraphicsItem::getConnectionGraphicsItemCount() const {
    return connectionItems_.size();
}

void LinkDialogPropertyGraphicsItem::removeConnectionGraphicsItem(DialogConnectionGraphicsItem* cItem) {
    connectionItems_.erase(std::remove(connectionItems_.begin(), connectionItems_.end(), cItem), connectionItems_.end());
}

DialogConnectionGraphicsItem* LinkDialogPropertyGraphicsItem::getArrowConnectionAt(const QPointF pos) const {
    QPointF itemPos = mapFromScene(pos);

    for (size_t i=0; i<connectionItems_.size(); i++) {
        QRectF arrowRect = calculateArrowRect(i+1, true);

        if (arrowRect.contains(itemPos))
            return connectionItems_[i];
    }

    for (size_t i=0; i<connectionItems_.size(); i++) {
        QRectF arrowRect = calculateArrowRect(i+1, false);

        if (arrowRect.contains(itemPos))
            return connectionItems_[i];
    }

    return 0;
}

QRectF LinkDialogPropertyGraphicsItem::calculateArrowRect(size_t curPort, bool computeRight) const {
    QPointF arrowDim(arrowDimensionWidth, arrowDimensionHeight);
    QPointF rectDim(0, rect().height()/(getConnectionGraphicsItemCount()+1));
    qreal x = rect().right()-arrowDim.x();

    if (!computeRight)
        x = rect().left();

    qreal y = rect().top()+(curPort*rectDim.y()) - arrowDim.y();
    return QRectF(x, y, arrowDim.x(), 2*arrowDim.y());
}

QRectF LinkDialogPropertyGraphicsItem::calculateArrowRect(DialogConnectionGraphicsItem* cItem, bool computeRight) const {
    for (size_t i=0; i<connectionItems_.size(); i++) {
        if (connectionItems_[i]==cItem)
            return calculateArrowRect(i+1, computeRight);
    }

    return QRectF();
}

bool LinkDialogPropertyGraphicsItem::isArrowPointedRight(DialogConnectionGraphicsItem* cItem) {
    QPointF c = pos();
    QPointF bl = rect().bottomLeft();
    QPointF br = rect().bottomRight();
    QPointF propertyMappedDim ;
    propertyMappedDim = mapToParent(br) -  mapToParent(bl);
    QPointF rightBoundaryCenter = c + (propertyMappedDim/2.0);
    propertyMappedDim = mapToParent(bl) -  mapToParent(br);
    QPointF leftBoundaryCenter = c + (propertyMappedDim/2.0);
    qreal dist1 = std::min(QVector2D(rightBoundaryCenter - cItem->getStartPoint()).length(),
                           QVector2D(rightBoundaryCenter - cItem->getEndPoint()).length());
    qreal dist2 = std::min(QVector2D(leftBoundaryCenter - cItem->getStartPoint()).length(),
                           QVector2D(leftBoundaryCenter - cItem->getEndPoint()).length());

    if (dist1<dist2) return false;

    return true;
}

void LinkDialogPropertyGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    IVW_UNUSED_PARAM(options);
    IVW_UNUSED_PARAM(widget);
    p->setPen(Qt::black);
    p->setRenderHint(QPainter::Antialiasing, true);
    p->save();
    // paint property
    QLinearGradient grad(rect().topLeft(), rect().bottomLeft());

    if (isSelected()) {
        grad.setColorAt(0.0f, QColor(110,77,77));
        grad.setColorAt(0.2f, QColor(110,77,77));
        grad.setColorAt(1.0f, QColor(50,0,0));
    } else {        
        if (isCompositeSubProperty_) {
            grad.setColorAt(0.0f, QColor(127,127,102));
            grad.setColorAt(0.5f, QColor(127,127,102));
            grad.setColorAt(1.0f, QColor(127,127,102));
        } else {
            grad.setColorAt(0.0f, QColor(255,255,204));
            grad.setColorAt(0.5f, QColor(255,255,204));
            grad.setColorAt(1.0f, QColor(255,255,204));
        }
    }

    p->setBrush(grad);

    QPen blackPen(QColor(0, 0, 0), 3);
    QPen greyPen(QColor(96, 96, 96), 1);
    QRectF bRect = rect();
    
    QPainterPath roundRectPath;
    roundRectPath.moveTo(bRect.left(), bRect.top()+propertyRoundedCorners);
    roundRectPath.lineTo(bRect.left(), bRect.bottom()-propertyRoundedCorners);
    roundRectPath.arcTo(bRect.left(), bRect.bottom()-(2*propertyRoundedCorners), (2*propertyRoundedCorners), (2*propertyRoundedCorners), 180.0,
    90.0);
    roundRectPath.lineTo(bRect.right()-propertyRoundedCorners, bRect.bottom());
    roundRectPath.arcTo(bRect.right()-(2*propertyRoundedCorners), bRect.bottom()-(2*propertyRoundedCorners), (2*propertyRoundedCorners),
    (2*propertyRoundedCorners), 270.0, 90.0);
    roundRectPath.lineTo(bRect.right(), bRect.top()+propertyRoundedCorners);
    roundRectPath.arcTo(bRect.right()-(2*propertyRoundedCorners), bRect.top(), (2*propertyRoundedCorners), (2*propertyRoundedCorners), 0.0,
    90.0);
    roundRectPath.lineTo(bRect.left()+propertyRoundedCorners, bRect.top());
    roundRectPath.arcTo(bRect.left(), bRect.top(), (2*propertyRoundedCorners), (2*propertyRoundedCorners), 90.0, 90.0);
    p->drawPath(roundRectPath);
    
    QPainterPath roundRectPath_Top;
    QPainterPath roundRectPath_Left;
    QPainterPath roundRectPath_Bottom;
    QPainterPath roundRectPath_Right;   

    //Left
    p->setPen(blackPen);
    roundRectPath_Left.moveTo(bRect.left(), bRect.top());
    roundRectPath_Left.lineTo(bRect.left(), bRect.bottom());    
    p->drawPath(roundRectPath_Left);

    //Bottom
    if (!isBottomItem_ && !isTopItem_) p->setPen(greyPen);
    else if (isBottomItem_) p->setPen(blackPen);
    else p->setPen(greyPen);
    roundRectPath_Bottom.moveTo(bRect.left(), bRect.bottom());
    roundRectPath_Bottom.lineTo(bRect.right(), bRect.bottom());
    p->drawPath(roundRectPath_Bottom);

    //Right
    p->setPen(blackPen);
    roundRectPath_Right.moveTo(bRect.right(), bRect.bottom());
    roundRectPath_Right.lineTo(bRect.right(), bRect.top());
    p->drawPath(roundRectPath_Right);

    //Top
    if (!isBottomItem_ && !isTopItem_) p->setPen(greyPen);
    else if (isTopItem_)  p->setPen(blackPen);
    else p->setPen(greyPen);
    roundRectPath_Top.moveTo(bRect.left(), bRect.top());
    roundRectPath_Top.lineTo(bRect.right(), bRect.top());
    p->drawPath(roundRectPath_Top);
    
    p->restore();
    p->save();
    QPoint arrowDim(arrowDimensionWidth, arrowDimensionHeight);

    for (size_t i=0; i<connectionItems_.size(); i++) {
        //Determine if arrow need to be drawn pointing left or right side
        bool right = isArrowPointedRight(connectionItems_[i]);
        //If arrow points right, then get the rectangle aligned to the left-
        //boundary of property item (rectangle) and vice versa
        QRectF arrowRect = calculateArrowRect(connectionItems_[i], !right);
        QPainterPath rectPath;

        //determine color of start and end arrow
        if (connectionItems_[i]->getEndProperty()==this) {
            arrowRect = calculateArrowRect(connectionItems_[i]->getEndArrowHeadIndex(), !right);
            p->setPen(Qt::black);
            p->setBrush(Qt::green);
        }
        else if (connectionItems_[i]->getStartProperty()==this) {
            arrowRect = calculateArrowRect(connectionItems_[i]->getStartArrowHeadIndex(), !right);
            p->setPen(Qt::transparent);
            p->setBrush(Qt::transparent);
        }

        if (connectionItems_[i]->isBidirectional()) {
            p->setPen(Qt::black);
            p->setBrush(Qt::green);
        }

        if (right) {
            rectPath.moveTo(arrowRect.left(), arrowRect.top());
            rectPath.lineTo(arrowRect.left(), arrowRect.bottom());
            rectPath.lineTo(arrowRect.right(), arrowRect.bottom()-arrowRect.height()/2);
            rectPath.closeSubpath();
        }
        else {
            rectPath.moveTo(arrowRect.right(), arrowRect.top());
            rectPath.lineTo(arrowRect.right(), arrowRect.bottom());
            rectPath.lineTo(arrowRect.left(), arrowRect.bottom()-arrowRect.height()/2);
            rectPath.closeSubpath();
        }

        p->drawPath(rectPath);
    }

    p->restore();
}

QVariant LinkDialogPropertyGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value) {
    return QGraphicsItem::itemChange(change, value);
}

void LinkDialogPropertyGraphicsItem::setProperty(Property* prop) {
    setGraphicsItemData(prop);

    if (prop) {
        std::string className = prop->getClassIdentifier();
        className = removeSubString(className, "Property");
        QString label = QString::fromStdString(prop->getDisplayName());
        classLabel_->setText(label);
        typeLabel_->setText(QString::fromStdString(className));
    } else {
        classLabel_->setText("");
        typeLabel_->setText("");
    }
}

QPointF LinkDialogPropertyGraphicsItem::getShortestBoundaryPointTo(LinkDialogPropertyGraphicsItem* inProperty) {
    return getShortestBoundaryPointTo(inProperty->pos());
}

QPointF LinkDialogPropertyGraphicsItem::getShortestBoundaryPointTo(QPointF inPos) {
    QPointF c = pos();
    QPointF bl = rect().bottomLeft();
    QPointF br = rect().bottomRight();
    QPointF propertyMappedDim ;
    propertyMappedDim = mapToParent(br) -  mapToParent(bl);
    QPointF rightBoundaryCenter1 = c + (propertyMappedDim/2.0);
    propertyMappedDim = mapToParent(bl) -  mapToParent(br);
    QPointF leftBoundaryCenter1 = c + (propertyMappedDim/2.0);
    QVector2D vec1(leftBoundaryCenter1 - inPos);
    QVector2D vec2(rightBoundaryCenter1 - inPos);

    if (vec1.length()>vec2.length())
        return rightBoundaryCenter1;

    return leftBoundaryCenter1;
}

QPointF LinkDialogPropertyGraphicsItem::calculateArrowCenter(size_t curPort, bool computeRight) const {
    size_t arrowCount = getConnectionGraphicsItemCount();
    QPointF o = pos() ;

    if (computeRight) {
        QPointF br = o + QPointF(rect().width()/2, rect().height()/2);
        QPointF tr = o + QPointF(rect().width()/2, -rect().height()/2);
        QPointF vec(br - tr);
        vec = vec/(arrowCount+1) ;

        if (arrowCount==0) vec = vec/2;

        vec*=curPort;
        return tr+vec;
    }

    QPointF bl = o + QPointF(-rect().width()/2, rect().height()/2);
    QPointF tl = o + QPointF(-rect().width()/2, -rect().height()/2);
    QPointF vec(bl - tl);
    vec = vec/(arrowCount+1) ;

    if (arrowCount==0) vec = vec/2;

    vec*=curPort;
    return tl+vec;
}

} //namespace