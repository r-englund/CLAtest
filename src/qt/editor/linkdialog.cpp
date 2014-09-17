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

#include <QMatrix>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QButtonGroup>
#include <QPushButton>
#include <QVector2D>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QLineEdit>



#include <inviwo/qt/editor/linkdialog.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/properties/compositeproperty.h>

#define IS_SUB_PROPERTY(prop) (prop->getOwner()->getProcessor() != prop->getOwner())
#define IS_COMPOSITE_PROPERTY(prop) dynamic_cast<CompositeProperty*>(prop)
#define EXPAND_SUB_PROPERTIES_BY_DEFAULT false

namespace inviwo {

static const int processorItemWidth = 175; //all other parameters depends on processor width.
static const int processorItemHeight = 50;
static const int processorRoundedCorners = 9;
static const int processorLabelHeight = 8;

static const int linkDialogWidth = processorItemWidth*4;
static const int linkDialogHeight = processorItemHeight*12;

static const int propertyItemWidth = processorItemWidth*3/4;
static const int propertyItemHeight = processorItemHeight;
static const int propertyRoundedCorners = 0;
static const int propertyLabelHeight = 8;

static const int arrowDimensionWidth = propertyItemWidth/10;
static const int arrowDimensionHeight =arrowDimensionWidth/2;


DialogCurveGraphicsItem::DialogCurveGraphicsItem(QPointF startPoint, QPointF endPoint, uvec3 color) :
    CurveGraphicsItem(startPoint, endPoint, color) {
    setZValue(LINKDIALOG_CONNECTION_GRAPHICSITEM_DEPTH);
}

DialogCurveGraphicsItem::~DialogCurveGraphicsItem() { }

QPainterPath DialogCurveGraphicsItem::obtainCurvePath(QPointF startPoint, QPointF endPoint) const {
    float delta = endPoint.x()-startPoint.x();
    QPointF ctrlPoint1 = QPointF(startPoint.x()+delta/4.0, startPoint.y());
    QPointF ctrlPoint2 = QPointF(endPoint.x()-delta/4.0, endPoint.y());
    QPainterPath bezierCurve;
    bezierCurve.moveTo(startPoint);
    bezierCurve.cubicTo(ctrlPoint1, ctrlPoint2, endPoint);
    return bezierCurve;
}

//////////////////////////////////////////////////////////////////////////

DialogConnectionGraphicsItem::DialogConnectionGraphicsItem(LinkDialogPropertyGraphicsItem* startProperty,
        LinkDialogPropertyGraphicsItem* endProperty,
        PropertyLink* propertyLink) :
    DialogCurveGraphicsItem(startProperty->getShortestBoundaryPointTo(endProperty),
                            endProperty->getShortestBoundaryPointTo(startProperty),
                            uvec3(38,38,38)),
    startPropertyGraphicsItem_(startProperty),
    endPropertyGraphicsItem_(endProperty),
    propertyLink_(propertyLink)
{
    setFlags(ItemIsSelectable | ItemIsFocusable);
    initialize();
}

DialogConnectionGraphicsItem::~DialogConnectionGraphicsItem() {
}

void DialogConnectionGraphicsItem::initialize() {
    startPropertyGraphicsItem_->addConnectionGraphicsItem(this);
    setStartArrowHeadIndex(startPropertyGraphicsItem_->getConnectionGraphicsItemCount());
    endPropertyGraphicsItem_->addConnectionGraphicsItem(this);
    setEndArrowHeadIndex(endPropertyGraphicsItem_->getConnectionGraphicsItemCount());
}

void DialogConnectionGraphicsItem::deinitialize() {
    startPropertyGraphicsItem_->removeConnectionGraphicsItem(this);
    endPropertyGraphicsItem_->removeConnectionGraphicsItem(this);
    propertyLink_ = NULL;
}

void DialogConnectionGraphicsItem::updateStartEndPoint() {
    QPoint arrowDim(arrowDimensionWidth, arrowDimensionHeight);
    //Start Property
    QPointF aCenterR = startPropertyGraphicsItem_->calculateArrowCenter(startArrowHeadIndex_, true);
    QPointF aCenterL = startPropertyGraphicsItem_->calculateArrowCenter(startArrowHeadIndex_, false);
    QPointF arrowCenter;
    QPointF start = getStartPoint();
    QVector2D vec1(aCenterR - start);
    QVector2D vec2(aCenterL - start);
    arrowCenter = aCenterR;

    if (vec2.length()<vec1.length())
        arrowCenter = aCenterL;

    setStartPoint(arrowCenter);
    //End Property
    aCenterR = endPropertyGraphicsItem_->calculateArrowCenter(endArrowHeadIndex_, true);
    aCenterL = endPropertyGraphicsItem_->calculateArrowCenter(endArrowHeadIndex_, false);
    QPointF end = getEndPoint();
    vec1 = QVector2D(aCenterR - end);
    vec2 = QVector2D(aCenterL - end);
    arrowCenter = aCenterR;

    if (vec2.length()<vec1.length())
        arrowCenter = aCenterL;

    setEndPoint(arrowCenter);
}

bool DialogConnectionGraphicsItem::isBidirectional() {
    return InviwoApplication::getPtr()->getProcessorNetwork()->getBidirectionalPair(
               propertyLink_->getSourceProperty(), propertyLink_->getDestinationProperty()) != 0;
}

void DialogConnectionGraphicsItem::switchDirection() {
    LinkDialogPropertyGraphicsItem* tempProp = getEndProperty();
    endPropertyGraphicsItem_ = getStartProperty();
    startPropertyGraphicsItem_ = tempProp;
    size_t temp = endArrowHeadIndex_;
    endArrowHeadIndex_ = startArrowHeadIndex_;
    startArrowHeadIndex_ = temp;

    if (propertyLink_) propertyLink_->switchDirection();
}

void DialogConnectionGraphicsItem::updateConnectionDrawing() {
    if (!propertyLink_) return;
    startPropertyGraphicsItem_->prepareGeometryChange();
    endPropertyGraphicsItem_->prepareGeometryChange();
    updateStartEndPoint();
}

void DialogConnectionGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e) {
    QGraphicsItem::mouseDoubleClickEvent(e);
}

/*---------------------------------------------------------------------------------------*/

LinkDialogProcessorGraphicsItem::LinkDialogProcessorGraphicsItem() : GraphicsItemData<Processor>() {
    setZValue(LINKDIALOG_PROCESSOR_GRAPHICSITEM_DEPTH);
    //setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable | ItemSendsGeometryChanges);
    setRect(-processorItemWidth/2, -processorItemHeight/2, processorItemWidth, processorItemHeight);
    QGraphicsDropShadowEffect* processorShadowEffect = new QGraphicsDropShadowEffect();
    processorShadowEffect->setOffset(3.0);
    processorShadowEffect->setBlurRadius(3.0);
    setGraphicsEffect(processorShadowEffect);
    nameLabel_ = new LabelGraphicsItem(this);
    nameLabel_->setPos(-processorItemWidth/2.0+processorLabelHeight/2.0, -processorItemHeight/2.0+processorLabelHeight);
    nameLabel_->setDefaultTextColor(Qt::white);
    nameLabel_->setFont(QFont("Segoe", processorLabelHeight, QFont::Black, false));
    nameLabel_->setCrop(9, 8);
    classLabel_ = new LabelGraphicsItem(this);
    classLabel_->setPos(-processorItemWidth/2.0+processorLabelHeight/2.0, -processorItemHeight/2.0+processorLabelHeight*2.5);
    classLabel_->setDefaultTextColor(Qt::lightGray);
    classLabel_->setFont(QFont("Segoe", processorLabelHeight, QFont::Normal, true));
    classLabel_->setCrop(9, 8);
}

LinkDialogProcessorGraphicsItem::~LinkDialogProcessorGraphicsItem() {}

QSizeF LinkDialogProcessorGraphicsItem::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
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

void LinkDialogProcessorGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    IVW_UNUSED_PARAM(options);
    IVW_UNUSED_PARAM(widget);
    p->save();
    p->setPen(Qt::black);
    p->setRenderHint(QPainter::Antialiasing, true);
    // paint processor
    QLinearGradient grad(rect().topLeft(), rect().bottomLeft());

    if (isSelected()) {
        grad.setColorAt(0.0f, QColor(110,77,77));
        grad.setColorAt(0.2f, QColor(110,77,77));
        grad.setColorAt(1.0f, QColor(50,38,38));
    } else {
        grad.setColorAt(0.0f, QColor(77,77,77));
        grad.setColorAt(0.2f, QColor(77,77,77));
        grad.setColorAt(1.0f, QColor(38,38,38));
    }

    p->setBrush(grad);
    QPainterPath roundRectPath;
    QRectF bRect = rect();
    roundRectPath.moveTo(bRect.left(), bRect.top()+processorRoundedCorners);
    roundRectPath.lineTo(bRect.left(), bRect.bottom()-processorRoundedCorners);
    roundRectPath.arcTo(bRect.left(), bRect.bottom()-(2*processorRoundedCorners), (2*processorRoundedCorners), (2*processorRoundedCorners),
                        180.0, 90.0);
    roundRectPath.lineTo(bRect.right()-processorRoundedCorners, bRect.bottom());
    roundRectPath.arcTo(bRect.right()-(2*processorRoundedCorners), bRect.bottom()-(2*processorRoundedCorners), (2*processorRoundedCorners),
                        (2*processorRoundedCorners), 270.0, 90.0);
    roundRectPath.lineTo(bRect.right(), bRect.top()+processorRoundedCorners);
    roundRectPath.arcTo(bRect.right()-(2*processorRoundedCorners), bRect.top(), (2*processorRoundedCorners), (2*processorRoundedCorners), 0.0,
                        90.0);
    roundRectPath.lineTo(bRect.left()+processorRoundedCorners, bRect.top());
    roundRectPath.arcTo(bRect.left(), bRect.top(), (2*processorRoundedCorners), (2*processorRoundedCorners), 90.0, 90.0);
    p->drawPath(roundRectPath);
    p->restore();
}



QVariant LinkDialogProcessorGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value) {
    return QGraphicsItem::itemChange(change, value);
}

void LinkDialogProcessorGraphicsItem::setProcessor(Processor* processor, bool expandProperties) {
    setGraphicsItemData(processor);

    if (processor) {
        nameLabel_->setText(QString::fromStdString(processor->getIdentifier()));
        classLabel_->setText(QString::fromStdString(processor->getClassIdentifier()));
        propertyGraphicsItems_.clear();
        std::vector<Property*> properties = processor->getProperties();

        for (size_t i=0; i<properties.size(); i++) {
            CompositeProperty* compProp = IS_COMPOSITE_PROPERTY(properties[i]);
            if (compProp && expandProperties) {
                //LogWarn("Found composite sub properties")
                std::vector<Property*> subProperties = compProp->getProperties();
                for (size_t j=0; j<subProperties.size(); j++) {
                    LinkDialogPropertyGraphicsItem* compItem = new LinkDialogPropertyGraphicsItem(this, subProperties[j]);                
                    propertyGraphicsItems_.push_back(compItem);
                }
            }
            else {
                propertyGraphicsItems_.push_back(new LinkDialogPropertyGraphicsItem(this, properties[i]));
            }
        }
    } else {
        nameLabel_->setText("");
        classLabel_->setText("");
    }
}

/*---------------------------------------------------------------------------------------*/

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

/*---------------------------------------------------------------------------------------*/

LinkDialogGraphicsScene::LinkDialogGraphicsScene(QWidget* parent)
    : QGraphicsScene(parent)
    , src_(0)
    , dest_(0)
    , linkCurve_(0)
    , startProperty_(0)
    , endProperty_(0)
    , processorNetwork_(0)
    , expandProperties_(false) {

    // The defalt bsp tends to crash...  
    setItemIndexMethod(QGraphicsScene::NoIndex);
}

QGraphicsItem* LinkDialogGraphicsScene::getPropertyGraphicsItemAt(Property* property) {
    LinkDialogPropertyGraphicsItem* graphicsItem = 0;

    for (size_t i = 0; i < processorGraphicsItems_.size(); i++) {
        std::vector<LinkDialogPropertyGraphicsItem*> propertyItems =
            processorGraphicsItems_[i]->getPropertyItemList();

        for (size_t j = 0; j < propertyItems.size(); j++) {
            if (propertyItems[j]->getGraphicsItemData() == property) {
                graphicsItem = propertyItems[j];
                break;
            }
        }
    }

    return graphicsItem;
}

void LinkDialogGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    LinkDialogPropertyGraphicsItem* tempProperty =
        getSceneGraphicsItemAt<LinkDialogPropertyGraphicsItem>(e->scenePos());

    if (!startProperty_ && tempProperty) {
        startProperty_ = tempProperty;

        if (linkCurve_) {
            linkCurve_->hide();
            removeItem(linkCurve_);
            delete linkCurve_;
            linkCurve_ = 0;
        }

        QPointF center = startProperty_->getShortestBoundaryPointTo(e->scenePos());
        linkCurve_ = new DialogCurveGraphicsItem(center, e->scenePos());
        linkCurve_->setZValue(2.0);
        addItem(linkCurve_);
        linkCurve_->show();
        e->accept();
    } else
        QGraphicsScene::mousePressEvent(e);

    return;
}

void LinkDialogGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (linkCurve_) {
        QPointF center = startProperty_->getShortestBoundaryPointTo(e->scenePos());
        linkCurve_->setStartPoint(center) ;
        linkCurve_->setEndPoint(e->scenePos());
        e->accept();
    } else
        QGraphicsScene::mouseMoveEvent(e);
}

void LinkDialogGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    if (linkCurve_) {
        delete linkCurve_;
        linkCurve_ = NULL;
        endProperty_ = getSceneGraphicsItemAt<LinkDialogPropertyGraphicsItem>(e->scenePos());

        if (endProperty_ && (endProperty_!=startProperty_)) {
            Property* sProp = startProperty_->getGraphicsItemData();
            Property* eProp = endProperty_->getGraphicsItemData();
            bool src2dst = SimpleCondition::canLink(sProp, eProp);
            bool dst2src = SimpleCondition::canLink(eProp, sProp);
            if (src2dst &&dst2src)
                addPropertyLink(sProp, eProp, true);
            else if (src2dst)
                addPropertyLink(sProp, eProp, false);
            else if (dst2src)
                addPropertyLink(eProp, sProp, false);
        }

        startProperty_ = NULL;
        endProperty_ = NULL;
        e->accept();
    } else {
        QGraphicsScene::mouseReleaseEvent(e);
    }

    startProperty_ = NULL;
    endProperty_ = NULL;
}

void LinkDialogGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e) {
    LinkDialogPropertyGraphicsItem* propertyItem =
        getSceneGraphicsItemAt<LinkDialogPropertyGraphicsItem>(e->scenePos());

    if (propertyItem) {
        DialogConnectionGraphicsItem* propertyLink =
            propertyItem->getArrowConnectionAt(e->scenePos());

        if (propertyLink) {
            if (isPropertyLinkBidirectional(propertyLink)) {
                makePropertyLinkBidirectional(propertyLink, false);

                if (propertyLink->getStartProperty() == propertyItem)
                    switchPropertyLinkDirection(propertyLink);
            } else {
                if (propertyLink->getStartProperty() == propertyItem)
                    makePropertyLinkBidirectional(propertyLink, true);
            }
        }

        e->accept();
    } else
        QGraphicsScene::mouseDoubleClickEvent(e);
}

void LinkDialogGraphicsScene::addPropertyLink(Property* sProp, Property* eProp,
                                              bool bidirectional) {
    LinkDialogPropertyGraphicsItem* startProperty =
        qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemAt(sProp));
    LinkDialogPropertyGraphicsItem* endProperty =
        qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemAt(eProp));
    addPropertyLink(startProperty, endProperty, bidirectional);
    DialogConnectionGraphicsItem* propertyLinkItem =
        getConnectionGraphicsItem(startProperty, endProperty);

    if (bidirectional) makePropertyLinkBidirectional(propertyLinkItem, true);

    addConnectionToCurrentList(propertyLinkItem);
}

int LinkDialogGraphicsScene::currentLinkItemsCount() {
    return static_cast<int>(currentConnectionGraphicsItems_.size());
}

void LinkDialogGraphicsScene::setExpandProperties(bool expand) { 
    if (expand!=expandProperties_) {
        expandProperties_ = expand;
    }
}

void LinkDialogGraphicsScene::addPropertyLink(PropertyLink* propertyLink) {
    //For adding representations for existing links in the network
    //LogInfo("Adding Property Link.");
    LinkDialogPropertyGraphicsItem* startProperty =  qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemAt(
                propertyLink->getSourceProperty()));
    LinkDialogPropertyGraphicsItem* endProperty =  qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemAt(
                propertyLink->getDestinationProperty()));

    if (startProperty && endProperty)
        initializePorpertyLinkRepresentation(startProperty, endProperty, propertyLink);
}

void LinkDialogGraphicsScene::addPropertyLink(LinkDialogPropertyGraphicsItem* startProperty, LinkDialogPropertyGraphicsItem* endProperty, bool bidirectional) {
    //For adding new links to network and creating representations
    //LogInfo("Adding Property Link.");
    Property* sProp = startProperty->getGraphicsItemData();
    Property* eProp = endProperty->getGraphicsItemData();

    ///////////////////////////////////////////////////////////////////////
    //TODO: ProcessorLinks are Deprecated. To be removed

    /*
    PropertyOwner* startProcessor = src_;
    PropertyOwner* endProcessor = dest_;
    ProcessorLink* processorLink = processorNetwork_->getLink(startProcessor, endProcessor);

    if (processorLink && !processorLink->isLinked(sProp, eProp) && !processorLink->isLinked(eProp, sProp)) {
        processorLink->addPropertyLinks(sProp, eProp, startProcessor, endProcessor);
        PropertyLink* propertyLink = processorLink->getPropertyLink(sProp, eProp);

        if (propertyLink) initializePorpertyLinkRepresentation(startProperty, endProperty, propertyLink);
    }
    */

    if (!processorNetwork_->getLink(sProp, eProp)) {
        PropertyLink* propertyLink = processorNetwork_->addLink(sProp, eProp);
        if (propertyLink) initializePorpertyLinkRepresentation(startProperty, endProperty, propertyLink);
    }
}

void LinkDialogGraphicsScene::removeCurrentPropertyLinks() {
    DialogConnectionGraphicsItem* propertyLink=0;
    std::vector<DialogConnectionGraphicsItem*> tempList = currentConnectionGraphicsItems_;
    foreach(propertyLink, currentConnectionGraphicsItems_)
    removeConnectionFromCurrentList(propertyLink);
    foreach(propertyLink, tempList)
    removePropertyLink(propertyLink);
}

void LinkDialogGraphicsScene::removeAllPropertyLinks() {
    DialogConnectionGraphicsItem* propertyLink=0;
    std::vector<DialogConnectionGraphicsItem*> tempList = connectionGraphicsItems_;
    foreach(propertyLink, connectionGraphicsItems_)
    removeConnectionFromCurrentList(propertyLink);
    foreach(propertyLink, tempList)
    removePropertyLink(propertyLink);
}

void LinkDialogGraphicsScene::removePropertyLink(DialogConnectionGraphicsItem* propertyLink) {
    //LogInfo("Removing Property Link.");
    LinkDialogPropertyGraphicsItem* startProperty = propertyLink->getStartProperty();
    LinkDialogPropertyGraphicsItem* endProperty = propertyLink->getEndProperty();

    ///////////////////////////////////////////////////////////////////////
    //TODO: ProcessorLinks are Deprecated. To be removed

    /*
    PropertyOwner* startProcessor = src_;
    PropertyOwner* endProcessor = dest_;
    ProcessorLink* processorLink = processorNetwork_->getLink(startProcessor, endProcessor);

    if (processorLink->isLinked(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData())) {
        propertyLink->hide();
        connectionGraphicsItems_.erase(std::remove(connectionGraphicsItems_.begin(), connectionGraphicsItems_.end(), propertyLink),
                                       connectionGraphicsItems_.end());
        removeItem(propertyLink);
        propertyLink->deinitialize();
        cleanupAfterRemoveLink(propertyLink);
        delete propertyLink;
        processorLink->removePropertyLinks(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());
        startProperty->prepareGeometryChange();
        endProperty->prepareGeometryChange();
    }
    */

    ProcessorLink* processorLink = processorNetwork_->getProcessorLink(src_, dest_);
    if (processorLink) {
        processorLink->removePropertyLinks(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());
        processorLink->removePropertyLinks(endProperty->getGraphicsItemData(), startProperty->getGraphicsItemData());
    }

    PropertyLink* link = processorNetwork_->getLink(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());
    if (link) {
        propertyLink->hide();
        connectionGraphicsItems_.erase(std::remove(connectionGraphicsItems_.begin(), connectionGraphicsItems_.end(), propertyLink),
            connectionGraphicsItems_.end());
        removeItem(propertyLink);
        propertyLink->deinitialize();
        cleanupAfterRemoveLink(propertyLink);
        delete propertyLink;
        processorNetwork_->removeLink(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());
        processorNetwork_->removeLink(endProperty->getGraphicsItemData(), startProperty->getGraphicsItemData());
        startProperty->prepareGeometryChange();
        endProperty->prepareGeometryChange();
    }
}

void LinkDialogGraphicsScene::cleanupAfterRemoveLink(DialogConnectionGraphicsItem* propertyLink) {
    LinkDialogPropertyGraphicsItem* startProperty = propertyLink->getStartProperty();
    LinkDialogPropertyGraphicsItem* endProperty = propertyLink->getEndProperty();

    //re-assign connection ids
    for (size_t i=0; i<connectionGraphicsItems_.size(); i++) {
        if (connectionGraphicsItems_[i] == propertyLink) continue;
        else if (connectionGraphicsItems_[i]->getStartProperty() == startProperty) {
            size_t index = connectionGraphicsItems_[i]->getStartArrowHeadIndex();

            if (index > propertyLink->getStartArrowHeadIndex())
                connectionGraphicsItems_[i]->setStartArrowHeadIndex(index-1);
        }
        else if (connectionGraphicsItems_[i]->getEndProperty() == startProperty) {
            size_t index = connectionGraphicsItems_[i]->getEndArrowHeadIndex();

            if (index > propertyLink->getStartArrowHeadIndex())
                connectionGraphicsItems_[i]->setEndArrowHeadIndex(index-1);
        }
    }

    for (size_t i=0; i<connectionGraphicsItems_.size(); i++) {
        if (connectionGraphicsItems_[i] == propertyLink) continue;
        else if (connectionGraphicsItems_[i]->getEndProperty() == endProperty) {
            size_t index = connectionGraphicsItems_[i]->getEndArrowHeadIndex();

            if (index > propertyLink->getEndArrowHeadIndex())
                connectionGraphicsItems_[i]->setEndArrowHeadIndex(index-1);
        }
        else if (connectionGraphicsItems_[i]->getStartProperty() == endProperty) {
            size_t index = connectionGraphicsItems_[i]->getStartArrowHeadIndex();

            if (index > propertyLink->getEndArrowHeadIndex())
                connectionGraphicsItems_[i]->setStartArrowHeadIndex(index-1);
        }
    }

    for (size_t i=0; i<connectionGraphicsItems_.size(); i++)
        connectionGraphicsItems_[i]->updateConnectionDrawing();
}

bool LinkDialogGraphicsScene::isPropertyLinkBidirectional(DialogConnectionGraphicsItem* propertyLink) {
    LinkDialogPropertyGraphicsItem* startProperty = propertyLink->getStartProperty();
    LinkDialogPropertyGraphicsItem* endProperty = propertyLink->getEndProperty();

    ///////////////////////////////////////////////////////////////////////
    //TODO: ProcessorLinks are Deprecated. To be removed

    /*
    PropertyOwner* startProcessor = src_;
    PropertyOwner* endProcessor = dest_;
    ProcessorLink* processorLink = processorNetwork_->getLink(startProcessor, endProcessor);
    PropertyLink* propLink = processorLink->getPropertyLink(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());
    return processorLink->getBidirectionalPair(propLink)!=0;
    */
    return processorNetwork_->getBidirectionalPair(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData())!=0;
}

void LinkDialogGraphicsScene::makePropertyLinkBidirectional(DialogConnectionGraphicsItem* propertyLink, bool isBidirectional) {
    LinkDialogPropertyGraphicsItem* startProperty = propertyLink->getStartProperty();
    LinkDialogPropertyGraphicsItem* endProperty = propertyLink->getEndProperty();

    ///////////////////////////////////////////////////////////////////////
    //TODO: ProcessorLinks are Deprecated. To be removed

    /*
    PropertyOwner* startProcessor = src_;
    PropertyOwner* endProcessor = dest_;
    ProcessorLink* processorLink = processorNetwork_->getLink(startProcessor, endProcessor);
    PropertyLink* propLink = processorLink->getPropertyLink(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());

    if (isBidirectional) {
        if (processorLink->getBidirectionalPair(propLink)==0)
            processorLink->addPropertyLinks(endProperty->getGraphicsItemData(), startProperty->getGraphicsItemData(), endProcessor, startProcessor);
    }
    else {
        if (processorLink->getBidirectionalPair(propLink))
            processorLink->removeBidirectionalPair(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());
    }
    */

    PropertyLink* propLink = processorNetwork_->getLink(endProperty->getGraphicsItemData(), startProperty->getGraphicsItemData());
    if (isBidirectional) {
        if (!propLink) processorNetwork_->addLink(endProperty->getGraphicsItemData(), startProperty->getGraphicsItemData());
    }
    else {
        if (propLink) processorNetwork_->removeLink(endProperty->getGraphicsItemData(), startProperty->getGraphicsItemData());
        }

    propertyLink->updateConnectionDrawing();
}

void LinkDialogGraphicsScene::switchPropertyLinkDirection(DialogConnectionGraphicsItem* propertyLink) {
    if (!propertyLink->isBidirectional()) {
        propertyLink->switchDirection();
        propertyLink->updateConnectionDrawing();
    }

    update();
}

void LinkDialogGraphicsScene::addConnectionToCurrentList(DialogConnectionGraphicsItem* propertyLink) {
    if (std::find(currentConnectionGraphicsItems_.begin(), currentConnectionGraphicsItems_.end(),
                  propertyLink)==currentConnectionGraphicsItems_.end())
        currentConnectionGraphicsItems_.push_back(propertyLink);
}

void LinkDialogGraphicsScene::removeConnectionFromCurrentList(DialogConnectionGraphicsItem* propertyLink) {
    currentConnectionGraphicsItems_.erase(std::remove(currentConnectionGraphicsItems_.begin(), currentConnectionGraphicsItems_.end(),
                                          propertyLink),
                                          currentConnectionGraphicsItems_.end());
}

DialogConnectionGraphicsItem* LinkDialogGraphicsScene::getConnectionGraphicsItem(LinkDialogPropertyGraphicsItem* outProperty,
        LinkDialogPropertyGraphicsItem* inProperty) {
    for (size_t i=0; i<connectionGraphicsItems_.size(); i++) {
        if ((connectionGraphicsItems_[i]->getStartProperty() == outProperty && connectionGraphicsItems_[i]->getEndProperty() == inProperty) ||
            (connectionGraphicsItems_[i]->getStartProperty() == inProperty && connectionGraphicsItems_[i]->getEndProperty() == outProperty))
            return connectionGraphicsItems_[i];
    }

    return 0;
}

void LinkDialogGraphicsScene::initializePorpertyLinkRepresentation(LinkDialogPropertyGraphicsItem* outProperty,
        LinkDialogPropertyGraphicsItem* inProperty,
        PropertyLink* propertyLink) {

    ///////////////////////////////////////////////////////////////////////
    //TODO: ProcessorLinks are Deprecated. To be removed

    /*
    PropertyOwner* startProcessor = src_;
    PropertyOwner* endProcessor = dest_;
    ProcessorLink* processorLink = processorNetwork_->getLink(startProcessor, endProcessor);
    DialogConnectionGraphicsItem* cItem = new DialogConnectionGraphicsItem(outProperty, inProperty, propertyLink, processorLink);
    addItem(cItem);
    cItem->show();
    connectionGraphicsItems_.push_back(cItem);

    for (size_t i=0; i<connectionGraphicsItems_.size(); i++)
        connectionGraphicsItems_[i]->updateConnectionDrawing();
    */

    DialogConnectionGraphicsItem* cItem = new DialogConnectionGraphicsItem(outProperty, inProperty, propertyLink);
    addItem(cItem);
    cItem->show();
    connectionGraphicsItems_.push_back(cItem);

    for (size_t i=0; i<connectionGraphicsItems_.size(); i++)
        connectionGraphicsItems_[i]->updateConnectionDrawing();
}

void LinkDialogGraphicsScene::keyPressEvent(QKeyEvent* e) {
    if (e->key() == Qt::Key_Delete) {
        QList<QGraphicsItem*> selectedGraphicsItems = selectedItems();

        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            DialogConnectionGraphicsItem* connectionGraphicsItem = qgraphicsitem_cast<DialogConnectionGraphicsItem*>(selectedGraphicsItems[i]);

            if (connectionGraphicsItem) {
                removeConnectionFromCurrentList(connectionGraphicsItem);
                removePropertyLink(connectionGraphicsItem);
            }
        }
    }

    QGraphicsScene::keyPressEvent(e);
}

void LinkDialogGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* e) {
    DialogConnectionGraphicsItem* linkGraphicsItem = getSceneGraphicsItemAt<DialogConnectionGraphicsItem>(e->scenePos()) ;

    if (linkGraphicsItem && linkGraphicsItem->getPropertyLink()) {
        QMenu menu;
        QAction* deleteAction = menu.addAction("Delete");
        QAction* biDirectionAction = menu.addAction("BiDirectional");
        biDirectionAction->setCheckable(true);
        QAction* switchDirection = menu.addAction("Switch Direction");

        if (isPropertyLinkBidirectional(linkGraphicsItem))
            biDirectionAction->setChecked(true);
        else
            biDirectionAction->setChecked(false);

        QAction* result = menu.exec(QCursor::pos());

        if (result == deleteAction) {
            removeConnectionFromCurrentList(linkGraphicsItem);
            removePropertyLink(linkGraphicsItem);
        }
        else if (result == biDirectionAction) {
            if (biDirectionAction->isChecked())
                makePropertyLinkBidirectional(linkGraphicsItem, true);
            else
                makePropertyLinkBidirectional(linkGraphicsItem, false);
        }
        else if (result == switchDirection)
            switchPropertyLinkDirection(linkGraphicsItem);
    }
}

void LinkDialogGraphicsScene::initScene(std::vector<Processor*> srcProcessorList, std::vector<Processor*> dstProcessorList) {
    int xPosition = linkDialogWidth/4;
    int yPosition = processorItemHeight;
    int xIncrement = linkDialogWidth/2;
    int yIncrement = processorItemHeight;

    for (size_t i=0; i<srcProcessorList.size(); i++) {
        addProcessorsItemsToScene(srcProcessorList[i], xPosition, yPosition);
        yPosition+=yIncrement;
    }

    xPosition+=xIncrement;
    yPosition = processorItemHeight;

    for (size_t i=0; i<dstProcessorList.size(); i++) {
        addProcessorsItemsToScene(dstProcessorList[i], xPosition, yPosition);
        yPosition+=yIncrement;
    }

    ///////////////////////////////////////////////////////////////////////
    //TODO: ProcessorLinks are Deprecated. To be removed

    /*
    std::vector<ProcessorLink*> processorLinks = processorNetwork_->getLinks();

    for (size_t i=0; i<processorLinks.size(); i++) {
        PropertyOwner* srcProcessor = processorLinks[i]->getSourceProcessor();
        PropertyOwner* dstProcessor = processorLinks[i]->getDestinationProcessor();

        if (std::find(srcProcessorList.begin(), srcProcessorList.end(), srcProcessor) == srcProcessorList.end() &&
            std::find(dstProcessorList.begin(), dstProcessorList.end(), srcProcessor) == dstProcessorList.end())
            continue;

        if (std::find(srcProcessorList.begin(), srcProcessorList.end(), dstProcessor) == srcProcessorList.end() &&
            std::find(dstProcessorList.begin(), dstProcessorList.end(), dstProcessor) == dstProcessorList.end())
            continue;

        //inProcessor, outProcessor cannot be in the same processor list. But that case can never occur
        std::vector<PropertyLink*> propertyLinks = processorLinks[i]->getPropertyLinks();
        std::vector<PropertyLink*> pairList;
        PropertyLink* pair=0;

        for (size_t j=0; j<propertyLinks.size(); j++) {
            if (std::find(pairList.begin(), pairList.end(), propertyLinks[j])==pairList.end()) {
                addPropertyLink(propertyLinks[j]);
                pair = processorLinks[i]->getBidirectionalPair(propertyLinks[j]);

                if (pair) pairList.push_back(pair);
            }
        }
    }
    */

    for (size_t i=0; i<srcProcessorList.size(); i++) {
        PropertyOwner* srcProcessor = srcProcessorList[i];
        PropertyOwner* dstProcessor = dstProcessorList[i];
        std::vector<PropertyLink*> networkLinks = processorNetwork_->getLinksBetweenProcessors(srcProcessor, dstProcessor);

        std::vector<PropertyLink*> pairList;
        PropertyLink* pair=0;

        std::vector<PropertyLink*> propertyLinks; //uni directional links only

        for (size_t j=0; j<networkLinks.size(); j++) {
            if (std::find(pairList.begin(), pairList.end(), networkLinks[j])==pairList.end()) {
                propertyLinks.push_back(networkLinks[j]);
                pair =  processorNetwork_->getBidirectionalPair(networkLinks[j]->getSourceProperty(), networkLinks[j]->getDestinationProperty());
                if (pair) pairList.push_back(pair);
            }
        }

        pairList.clear();
        for (size_t j=0; j<propertyLinks.size(); j++) {
            if (std::find(pairList.begin(), pairList.end(), propertyLinks[j])==pairList.end()) 
            {

                CompositeProperty* compositeSrcProperty = IS_COMPOSITE_PROPERTY(propertyLinks[j]->getSourceProperty());
                CompositeProperty* compositeDstProperty = IS_COMPOSITE_PROPERTY(propertyLinks[j]->getDestinationProperty());

                if ( compositeSrcProperty && compositeDstProperty && expandProperties_) {
                    //LogWarn("Removing Composite Property Link. Adding Sub-Property Links")
                    bool bidirectional = false;
                    pair =  processorNetwork_->getBidirectionalPair(propertyLinks[j]->getSourceProperty(), propertyLinks[j]->getDestinationProperty());
                    if (pair) {
                        bidirectional = true;
                    }

                    //remove composite property link and add sub property links
                    //TODO: Recursive composite properties yet to be detected here

                    std::vector<Property*> srcProperties = compositeSrcProperty->getProperties();
                    std::vector<Property*> dstProperties = compositeDstProperty->getProperties();

                    Property* s = propertyLinks[j]->getSourceProperty(); 
                    Property* d = propertyLinks[j]->getDestinationProperty();
                    processorNetwork_->removeLink(s, d);
                    processorNetwork_->removeLink(d, s);

                   //Two different sub-properties can be linked which is not allowed now.
                   if (srcProperties.size() == dstProperties.size()) {
                       for (size_t k=0; k<srcProperties.size(); k++)
                            addPropertyLink(srcProperties[k], dstProperties[k], bidirectional);
                   }
                   else {
                       //TODO: Perform auto link?
                       LogWarn("Unable to link compsite sub-properties")
                   }
                }
                else {
                    bool isSubProperty = IS_SUB_PROPERTY(propertyLinks[j]->getSourceProperty());

                    if (isSubProperty && !expandProperties_) {
                        bool isBirectional = false;

                        Property* compsrc = getParentCompositeProperty(propertyLinks[j]->getSourceProperty(), srcProcessorList[i]);
                        Property* compdst = getParentCompositeProperty(propertyLinks[j]->getDestinationProperty(), dstProcessorList[i]);

                        if (compsrc && compdst) {
                            CompositeProperty* compositeSrcProperty = IS_COMPOSITE_PROPERTY(compsrc);
                            CompositeProperty* compositeDstProperty = IS_COMPOSITE_PROPERTY(compdst);

                            std::vector<Property*> srcProperties = compositeSrcProperty->getProperties();
                            std::vector<Property*> dstProperties = compositeDstProperty->getProperties();

                            if (srcProperties.size() == dstProperties.size()) {
                                //LogWarn("Remove Sub-Property Links. Add Composite Property Link")
                                for (size_t k=0; k<srcProperties.size(); k++) {

                                    if (srcProperties[k] == propertyLinks[j]->getSourceProperty() &&
                                        dstProperties[k] == propertyLinks[j]->getDestinationProperty()) {
                                        pair =  processorNetwork_->getBidirectionalPair(srcProperties[k], dstProperties[k]);
                                        if (pair) isBirectional = true;
                                        processorNetwork_->removeLink(srcProperties[k], dstProperties[k]);
                                        if (pair) processorNetwork_->removeLink(dstProperties[k], srcProperties[k]);
                                    }
                                }

                                if (!processorNetwork_->getLink(compsrc, compdst)) {
                                    addPropertyLink(compositeSrcProperty, compositeDstProperty, isBirectional);
                                    pair =  processorNetwork_->getBidirectionalPair(compositeSrcProperty, compositeDstProperty);
                                    if (pair) pairList.push_back(pair);
                                }
                            }
                        }
                    }
                    else {
                        //LogWarn("Just adding the sub-properties as it is")
                        addPropertyLink(propertyLinks[j]);
                        pair =  processorNetwork_->getBidirectionalPair(propertyLinks[j]->getSourceProperty(), propertyLinks[j]->getDestinationProperty());
                        if (pair) pairList.push_back(pair);
                    }
                }
            }
        }
    }

    currentConnectionGraphicsItems_.clear();
}

Property* LinkDialogGraphicsScene::getParentCompositeProperty(Property* subProperty, Processor* processor) {
    //uses recursion
    std::vector<Property*> properties = processor->getProperties();
    std::vector<CompositeProperty*> compositeProperties;

    for (size_t i=0; i<properties.size(); i++) {
        if (properties[i] == subProperty) return NULL; //not a composite property
        CompositeProperty* comp = IS_COMPOSITE_PROPERTY(properties[i]);
        if (comp)
            compositeProperties.push_back(comp);
    }

    while(compositeProperties.size()) {
        std::vector<CompositeProperty*> newComposites;
        for (size_t i=0; i<compositeProperties.size(); i++) {
            CompositeProperty *comp = compositeProperties[i];
            std::vector<Property*> p = comp->getProperties();
            for (size_t j=0; j<p.size(); j++) {
                if (p[j]==subProperty) return comp;

                CompositeProperty* c = IS_COMPOSITE_PROPERTY(p[j]);
                if (c) {
                    newComposites.push_back(c);
                }
            }
        }
        compositeProperties = newComposites;
    };

    return NULL;
}

void LinkDialogGraphicsScene::clearSceneRepresentations() {
    processorGraphicsItems_.clear();
    connectionGraphicsItems_.clear();
    currentConnectionGraphicsItems_.clear();
    clear();
}

void LinkDialogGraphicsScene::addProcessorsItemsToScene(Processor* processor, int xPosition, int yPosition) {
    LinkDialogProcessorGraphicsItem* procGraphicsItem = new LinkDialogProcessorGraphicsItem();
    processorGraphicsItems_.push_back(procGraphicsItem);
    //procGraphicsItem->setPos(xPosition, yPosition);
    //There is only one view which owns this scene
    //TODO: Any other better ways to map to current scene.
    //Verify if procGraphicsItem can map to current scene
    QList<QGraphicsView*> allViews = views();

    if (!allViews.size()) {
        //should not come here
        return;
    }

    procGraphicsItem->setPos(allViews[0]->mapToScene(xPosition, yPosition));
    procGraphicsItem->setProcessor(processor, expandProperties_);
    addItem(procGraphicsItem);
    procGraphicsItem->show();
    std::vector<LinkDialogPropertyGraphicsItem*> propItems = procGraphicsItem->getPropertyItemList();

    for (size_t i=0; i<propItems.size(); i++) {
        addItem(propItems[i]);
        propItems[i]->show();
        propItems[i]->updatePositionBasedOnProcessor(expandProperties_);
    }
}

void LinkDialogGraphicsScene::updatePropertyItemsOfAllProcessors() {
    //make sure link items are handled before calling this function
    LinkDialogProcessorGraphicsItem* procGraphicsItem;
    foreach(procGraphicsItem, processorGraphicsItems_) {
        Processor* processor = procGraphicsItem->getProcessor();
        {
            std::vector<LinkDialogPropertyGraphicsItem*> propItems = procGraphicsItem->getPropertyItemList();
            for (size_t i=0; i<propItems.size(); i++) {
                propItems[i]->hide();
                removeItem(propItems[i]);
            }

            procGraphicsItem->setProcessor(processor, expandProperties_);
            propItems = procGraphicsItem->getPropertyItemList();
            for (size_t i=0; i<propItems.size(); i++) {
                addItem(propItems[i]);
                propItems[i]->show();
                propItems[i]->updatePositionBasedOnProcessor(expandProperties_);
            }
        }
    }
}


/*---------------------------------------------------------------------------------------*/

LinkDialogGraphicsView::LinkDialogGraphicsView(QWidget* parent) : QGraphicsView(parent), scene_(0) {
    setRenderHint(QPainter::Antialiasing, true);
    setMouseTracking(true);
    setDragMode(QGraphicsView::RubberBandDrag);
}

void LinkDialogGraphicsView::setDialogScene(LinkDialogGraphicsScene* scene) {
    scene->setBackgroundBrush(QBrush(Qt::lightGray));
    setScene(scene);
    scene_ = scene;
}

void LinkDialogGraphicsView::mouseDoubleClickEvent(QMouseEvent* e) {
    QGraphicsView::mouseDoubleClickEvent(e);
}


LinkDialogGraphicsView::~LinkDialogGraphicsView() {}

/*---------------------------------------------------------------------------------------*/

LinkDialog::LinkDialog(QWidget* parent) : InviwoDockWidget("Edit Processor Link Dialog", parent) {
    IVW_UNUSED_PARAM(parent);
    //Handle multiple processor links here
}

LinkDialog::~LinkDialog() {}

LinkDialog::LinkDialog(Processor* src, Processor* dest, QWidget* parent) :InviwoDockWidget("Edit Processor Link Dialog", parent) {    
    std::vector<Processor*> srcList, dstList;
    srcList.push_back(src);
    dstList.push_back(dest);
    src_ = src;
    dest_ = dest;
    initDialog();
    linkDialogScene_->setNetwork(InviwoApplication::getPtr()->getProcessorNetwork()); //Network is required to add property links created in dialog (or remove )
    linkDialogScene_->setExpandProperties(EXPAND_SUB_PROPERTIES_BY_DEFAULT);
    linkDialogScene_->initScene(srcList, dstList);
}

void LinkDialog::initDialog() {
    setFloating(true);
    setWindowModality (Qt::ApplicationModal);   
    /*
    //Custom title bar for consistency with other docked widgets
    QString style("padding-top: 5px; \
                   padding-left: 5px; \
                   border-radius: 3px; \
                   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 #888888, stop:0.4 #555555, stop:0.5 #000000, stop:1 #111111);"
                   );
    
     QFrame* titleBar = new QFrame();
     QVBoxLayout* lableLayout = new QVBoxLayout(titleBar);
     QLabel *titleText = new QLabel("Edit Processor Link Dialog");
     titleText->setStyleSheet(style);
     lableLayout->addWidget(titleText);
     setTitleBarWidget(titleBar);
     //titleBarWidget()->setStyleSheet(style);
     setWindowFlags(Qt::Window | Qt::FramelessWindowHint);*/
    
    setObjectName("LinkDialogWidget");
    //setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setAllowedAreas(Qt::NoDockWidgetArea);
    QFrame* frame = new QFrame();

    QSize rSize(linkDialogWidth, linkDialogHeight);
    setFixedSize(rSize);
    QVBoxLayout* mainLayout = new QVBoxLayout(frame);
    linkDialogView_ = new LinkDialogGraphicsView(this);
    linkDialogScene_ = new LinkDialogGraphicsScene(this);
    linkDialogScene_->src_ = src_;
    linkDialogScene_->dest_ = dest_;
    linkDialogView_->setDialogScene(linkDialogScene_);
    linkDialogView_->setSceneRect(0,0,rSize.width(), rSize.height()*5);
    linkDialogView_->fitInView(linkDialogView_->rect());
    linkDialogView_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mainLayout->addWidget(linkDialogView_);
    QHBoxLayout* commonButtonLayout = new QHBoxLayout;
    //auto link button
    QHBoxLayout* autoLinkPushButtonLayout = new QHBoxLayout;
    autoLinkPushButtonLayout->setAlignment(Qt::AlignLeft);
    //qt documentation
    //auto link button
    autoLinkPushButton_ = new QPushButton("AutoLink", this);
    connect(autoLinkPushButton_, SIGNAL(clicked()), this, SLOT(clickedAutoLinkPushButton()));
    autoLinkPushButtonLayout->addWidget(autoLinkPushButton_, 10);
    //checkable combo box
    std::vector<std::string> options;
    options.push_back(SimpleCondition::conditionName());
    options.push_back(PartiallyMatchingIdCondition::conditionName());
    autoLinkOptions_ = new CheckableQComboBox(this,"AutoLink Filter", options);
    autoLinkPushButtonLayout->addWidget(autoLinkOptions_, 20);
    //delete button
    deleteAllLinkPushButton_ = new QPushButton("Delete All", this);
    connect(deleteAllLinkPushButton_, SIGNAL(clicked()), this, SLOT(clickedDeleteAllLinksPushButton()));
    autoLinkPushButtonLayout->addWidget(deleteAllLinkPushButton_, 10);
    //expand composite
    expandCompositeOption_ = new QCheckBox("Expand Properties", this);
    expandCompositeOption_->setChecked(EXPAND_SUB_PROPERTIES_BY_DEFAULT);
    autoLinkPushButtonLayout->addWidget(expandCompositeOption_, 10);
    connect(expandCompositeOption_, SIGNAL(toggled(bool)), this, SLOT(expandCompositeProperties(bool)));
    commonButtonLayout->addLayout(autoLinkPushButtonLayout);

    //okay cancel button
    QHBoxLayout* okayCancelButtonLayout = new QHBoxLayout;
    okayCancelButtonLayout->setAlignment(Qt::AlignRight);
    //qt documentation
    okayCancelbuttonBox_ = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(okayCancelbuttonBox_, SIGNAL(accepted()), this, SLOT(clickedOkayButton()));
    connect(okayCancelbuttonBox_, SIGNAL(rejected()), this, SLOT(clickedCancelButton()));
    okayCancelButtonLayout->addWidget(okayCancelbuttonBox_);
    commonButtonLayout->addLayout(okayCancelButtonLayout);
    mainLayout->addLayout(commonButtonLayout);
    setWidget(frame);
}

void LinkDialog::clickedOkayButton() {

    ///////////////////////////////////////////////////////////////////////
    //TODO: ProcessorLinks are Deprecated. To be removed

    /*
    if (linkDialogScene_->currentLinkItemsCount()) {
        ProcessorLink* link = linkDialogScene_->getNetwork()->getLink(src_, dest_);

        if (link)
            link->setModifiedByPropertyOwner(src_);
    }
    */

    if (linkDialogScene_->currentLinkItemsCount()) {
        InviwoApplication::getPtr()->getProcessorNetwork()->setLinkModifiedByOwner(src_);
    }

    updateProcessorLinks();

    std::vector<PropertyLink*> propertyLinks = InviwoApplication::getPtr()->getProcessorNetwork()->getLinksBetweenProcessors(src_, dest_);
    for (size_t j=0; j<propertyLinks.size(); j++) {

        if (expandCompositeOption_->isChecked()) {
            //LogWarn("Invalidation of sub-properties set to invalid")
            if (IS_SUB_PROPERTY(propertyLinks[j]->getSourceProperty()))
                propertyLinks[j]->getSourceProperty()->setInvalidationLevel(PropertyOwner::INVALID_OUTPUT);
            if (IS_SUB_PROPERTY(propertyLinks[j]->getDestinationProperty()))
                propertyLinks[j]->getDestinationProperty()->setInvalidationLevel(PropertyOwner::INVALID_OUTPUT);
        }
        else {
            //LogWarn("Invalidation of sub-properties set to valid")
            setValidationLevelOfSubPropertiesOfCompositeProperties(propertyLinks[j]->getSourceProperty(), PropertyOwner::VALID);
            setValidationLevelOfSubPropertiesOfCompositeProperties(propertyLinks[j]->getDestinationProperty(), PropertyOwner::VALID);
        }
    }

    //accept();
    InviwoApplication::getPtr()->getProcessorNetwork()->updatePropertyLinkCaches();
    hide();
    eventLoop_.quit();
}

void LinkDialog::updateProcessorLinks() {
    ProcessorLink* processorLink = InviwoApplication::getPtr()->getProcessorNetwork()->getProcessorLink(src_, dest_);
    std::vector<PropertyLink*> propertyLinks = InviwoApplication::getPtr()->getProcessorNetwork()->getLinksBetweenProcessors(src_, dest_);
    //if (propertyLinks.size()) 
    {
        if (!processorLink) processorLink = InviwoApplication::getPtr()->getProcessorNetwork()->addLink(src_, dest_);
        processorLink->removeAllPropertyLinks();
        for (size_t j=0; j<propertyLinks.size(); j++) {
            processorLink->addPropertyLinks(propertyLinks[j]);
        }
    }
}

void LinkDialog::clickedCancelButton() {
    linkDialogScene_->removeCurrentPropertyLinks();
    //accept();
    updateProcessorLinks();
    hide();
    eventLoop_.quit();
}

void LinkDialog::closeEvent ( QCloseEvent * event ) {
   eventLoop_.quit();
}

void LinkDialog::setValidationLevelOfSubPropertiesOfCompositeProperties(Property* property, PropertyOwner::InvalidationLevel invalidationLevel) {
    CompositeProperty* compositeProperty = IS_COMPOSITE_PROPERTY(property);
    if (!compositeProperty) return;
    std::vector<Property*> subProps = compositeProperty->getProperties();
    for (size_t i=0; i<subProps.size(); i++) {
        subProps[i]->setInvalidationLevel(invalidationLevel);
    }
}

void LinkDialog::clickedAutoLinkPushButton() {
    std::vector<Property*> srcProperties = src_->getProperties();
    std::vector<Property*> dstProperties = dest_->getProperties();
    int selectedTypes = (int) NoLinkCondition;
    std::vector<std::string> selectedConditons = autoLinkOptions_->getCheckedItems();

    for (size_t i=0; i<selectedConditons.size(); i++) {
        if (selectedConditons[i] == SimpleCondition::conditionName())
            selectedTypes|=SimpleCondition::conditionType();

        if (selectedConditons[i] == PartiallyMatchingIdCondition::conditionName())
            selectedTypes|=PartiallyMatchingIdCondition::conditionType();
    }

    for (size_t i=0; i<srcProperties.size(); i++) {
        for (size_t j=0; j<dstProperties.size(); j++) {

            if (expandCompositeOption_->isChecked()) {
                if (AutoLinker::canLink(srcProperties[i], dstProperties[j], (LinkingConditions) selectedTypes)) {
                    CompositeProperty* compSrc = IS_COMPOSITE_PROPERTY(srcProperties[i]);
                    CompositeProperty* compDst = IS_COMPOSITE_PROPERTY(dstProperties[j]);
                    if ( compSrc && compDst) {
                        //If composite property then try to link sub-properties only
                        std::vector<Property*> s = compSrc->getProperties();
                        std::vector<Property*> d = compDst->getProperties();
                        for (size_t ii=0; ii<s.size(); ii++) {
                            for (size_t jj=0; jj<d.size(); jj++) {
                                if (AutoLinker::canLink(s[ii], d[jj], (LinkingConditions) selectedTypes)) {
                                    linkDialogScene_->addPropertyLink(s[ii], d[jj], true);
                                }
                            }
                        }
                    }
                }
            } else {
                if (AutoLinker::canLink(srcProperties[i], dstProperties[j], (LinkingConditions) selectedTypes))
                    linkDialogScene_->addPropertyLink(srcProperties[i], dstProperties[j], true);
            }
        }
    }
}

void LinkDialog::clickedDeleteAllLinksPushButton() {
    linkDialogScene_->removeAllPropertyLinks();
}

void LinkDialog::expandCompositeProperties(bool expand) {
    linkDialogScene_->setExpandProperties(expand);
    linkDialogScene_->clearSceneRepresentations();
    QSize rSize(linkDialogWidth, linkDialogHeight);
    linkDialogView_->setSceneRect(0,0,rSize.width(), rSize.height()*5);
    linkDialogView_->fitInView(linkDialogView_->rect());
    linkDialogView_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    std::vector<Processor*> srcList, dstList;
    srcList.push_back(src_);
    dstList.push_back(dest_);
    linkDialogScene_->initScene(srcList, dstList);
}

int LinkDialog::exec() {
    show();
    //connect(this, SIGNAL(destroy()), &eventLoop_, SLOT(quit()));
    return eventLoop_.exec();
}


CheckableQComboBox::CheckableQComboBox(QWidget *parent , std::string widgetName, std::vector<std::string> options) : QComboBox(parent),widgetName_(widgetName) {
    setEditable(true);
    lineEdit()->setReadOnly(true);
    stdandardModel_ = new QStandardItemModel(static_cast<int>(options.size()),1);

    for (size_t i=0; i<options.size(); i++) {
        QStandardItem* item = new QStandardItem(QString(options[i].c_str()));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Checked, Qt::CheckStateRole);
        stdandardModel_->setItem(static_cast<int>(i), 0, item);
        standardItems_.push_back(item);
    }

    setModel(stdandardModel_);
    lineEdit()->setText(QString(widgetName_.c_str()));
    connect(stdandardModel_, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onAutoLinkOptionChecked(const QModelIndex&,
            const QModelIndex&)));
}

CheckableQComboBox::~CheckableQComboBox(){
    delete stdandardModel_;
}

bool CheckableQComboBox::isItemChecked(int i) {
    if (i>(int)standardItems_.size())
        return false;

    QStandardItem* item = standardItems_[i];

    if (item->checkState() == Qt::Checked)
        return true;

    return false;
}

std::vector<std::string> CheckableQComboBox::getCheckedItems() {
    std::vector<std::string> checkedItemString;

    for (size_t i=0; i<standardItems_.size(); i++)
        if (isItemChecked(static_cast<int>(i)))
            checkedItemString.push_back(standardItems_[i]->text().toLocal8Bit().constData());

    return checkedItemString;
}

void CheckableQComboBox::onAutoLinkOptionChecked(const QModelIndex& tl, const QModelIndex& br) {
    if (isItemChecked(tl.row())) {
        //do some maintenance stuff here
    }

    lineEdit()->setText(QString(widgetName_.c_str()));
}


/*---------------------------------------------------------------------------------------*/

} //namespace