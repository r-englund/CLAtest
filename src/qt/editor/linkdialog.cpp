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
#include <inviwo/core/util/variant.h>

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

QPainterPath DialogCurveGraphicsItem::obtainCurvePath() const {
    float delta = endPoint_.x()-startPoint_.x();
    QPointF ctrlPoint1 = QPointF(startPoint_.x()+delta/4.0, startPoint_.y());
    QPointF ctrlPoint2 = QPointF(endPoint_.x()-delta/4.0, endPoint_.y());
    QPainterPath bezierCurve;
    bezierCurve.moveTo(startPoint_);
    bezierCurve.cubicTo(ctrlPoint1, ctrlPoint2, endPoint_);
    return bezierCurve;
}

void DialogCurveGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    CurveGraphicsItem::paint(p, options, widget);
}

DialogConnectionGraphicsItem::DialogConnectionGraphicsItem(LinkDialogPropertyGraphicsItem* startProperty,
        LinkDialogPropertyGraphicsItem* endProperty,
        PropertyLink* propertyLink,
        ProcessorLink* processorLink) :
    DialogCurveGraphicsItem(startProperty->getShortestBoundaryPointTo(endProperty),
                            endProperty->getShortestBoundaryPointTo(startProperty),
                            uvec3(38,38,38)),
    startPropertyGraphicsItem_(startProperty),
    endPropertyGraphicsItem_(endProperty),
    propertyLink_(propertyLink),
    processorLink_(processorLink)
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
    propertyLink_ = 0;
    processorLink_ = 0;
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

void DialogConnectionGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    DialogCurveGraphicsItem::paint(p, options, widget);
}

bool DialogConnectionGraphicsItem::isBidirectional() {
    if (!processorLink_ || !propertyLink_) return false;

    return processorLink_->getBidirectionalPair(propertyLink_->getSourceProperty(), propertyLink_->getDestinationProperty())!=0;
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
    if (!processorLink_ || !propertyLink_) return;

    startPropertyGraphicsItem_->prepareGeometryChange();
    endPropertyGraphicsItem_->prepareGeometryChange();
    update();
    updateStartEndPoint();
}

QRectF DialogConnectionGraphicsItem::boundingRect() const {
    QPointF startPoint = getStartPoint();
    QPointF endPoint = getEndPoint();
    QPointF topLeft = QPointF(std::min(startPoint.x(), endPoint.x()),
                              std::min(startPoint.y(), endPoint.y()));
    return QRectF(topLeft.x()-30.0, topLeft.y()-30.0,
                  abs(startPoint.x()-endPoint.x())+60.0,
                  abs(startPoint.y()-endPoint.y())+60);
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

void LinkDialogProcessorGraphicsItem::setProcessor(Processor* processor) {
    setGraphicsItemData(processor);

    if (processor) {
        nameLabel_->setText(QString::fromStdString(processor->getIdentifier()));
        classLabel_->setText(QString::fromStdString(processor->getClassName()));
        propertyGraphicsItems_.clear();
        std::vector<Property*> properties = processor->getProperties();

        for (size_t i=0; i<properties.size(); i++)
            propertyGraphicsItems_.push_back(new LinkDialogPropertyGraphicsItem(this, properties[i]));
    } else {
        nameLabel_->setText("");
        classLabel_->setText("");
    }
}

/*---------------------------------------------------------------------------------------*/

LinkDialogPropertyGraphicsItem::LinkDialogPropertyGraphicsItem(LinkDialogProcessorGraphicsItem* processor,
        Property* prop) : GraphicsItemData<Property>() {
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

void LinkDialogPropertyGraphicsItem::updatePositionBasedOnProcessor() {
    if (!processorGraphicsItem_) return;

    Processor* processor = processorGraphicsItem_->getProcessor();
    int ind = 0;
    Property* prop = getGraphicsItemData();

    if (prop) {
        std::vector<Property*> properties = processor->getProperties();

        for (size_t i=0; i<properties.size(); i++) {
            if (prop == properties[i])
                break;

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
        grad.setColorAt(0.0f, QColor(255,255,204));
        grad.setColorAt(0.5f, QColor(255,255,204));
        grad.setColorAt(1.0f, QColor(255,255,204));
    }

    p->setBrush(grad);
    QPainterPath roundRectPath;
    QRectF bRect = rect();
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
        //TODO: Is class name required?
        //std::string className = prop->getClassName();
        std::string className("");
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

LinkDialogGraphicsScene::LinkDialogGraphicsScene(QWidget* parent):QGraphicsScene(parent),
    linkCurve_(0),
    startProperty_(0),
    endProperty_(0),
    processorNetwork_(0)
{
}

QGraphicsItem* LinkDialogGraphicsScene::getPropertyGraphicsItemAt(Property* property) {
    LinkDialogPropertyGraphicsItem* graphicsItem=0;

    for (size_t i=0; i<processorGraphicsItems_.size(); i++) {
        std::vector<LinkDialogPropertyGraphicsItem*> propertyItems = processorGraphicsItems_[i]->getPropertyItemList();

        for (size_t j=0; j<propertyItems.size(); j++) {
            if (propertyItems[j]->getGraphicsItemData() == property) {
                graphicsItem = propertyItems[j];
                break;
            }
        }
    }

    return graphicsItem;
}

void LinkDialogGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* e) {
    LinkDialogPropertyGraphicsItem* tempProperty = getSceneGraphicsItemAt<LinkDialogPropertyGraphicsItem>(e->scenePos());

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
    }
    else
        QGraphicsScene::mousePressEvent(e);

    return;
}

void LinkDialogGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {
    if (linkCurve_) {
        QPointF center = startProperty_->getShortestBoundaryPointTo(e->scenePos());
        linkCurve_->setStartPoint(center) ;
        linkCurve_->setEndPoint(e->scenePos());
        linkCurve_->update();
        update();
        e->accept();
    } else
        QGraphicsScene::mouseMoveEvent(e);
}

void LinkDialogGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    if (linkCurve_) {
        linkCurve_->hide();
        removeItem(linkCurve_);
        delete linkCurve_;
        linkCurve_ = 0;
        endProperty_ = getSceneGraphicsItemAt<LinkDialogPropertyGraphicsItem>(e->scenePos());

        if (endProperty_ && (endProperty_!=startProperty_)) {
            Property* sProp = startProperty_->getGraphicsItemData();
            Property* eProp = endProperty_->getGraphicsItemData();
            if (SimpleCondition::canLink(sProp, eProp))
                addPropertyLink(sProp, eProp, true);
        }

        startProperty_ = 0;
        endProperty_ = 0;
        e->accept();
    } else
        QGraphicsScene::mouseReleaseEvent(e);

    startProperty_ = 0;
    endProperty_ = 0;
}

void LinkDialogGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e) {
    LinkDialogPropertyGraphicsItem* propertyItem = getSceneGraphicsItemAt<LinkDialogPropertyGraphicsItem>(e->scenePos());

    if (propertyItem) {
        DialogConnectionGraphicsItem* propertyLink = propertyItem->getArrowConnectionAt(e->scenePos());

        if (propertyLink) {
            if (isPropertyLinkBidirectional(propertyLink)) {
                makePropertyLinkBidirectional(propertyLink, false);

                if (propertyLink->getStartProperty() == propertyItem)
                    switchPropertyLinkDirection(propertyLink);
            }
            else {
                if (propertyLink->getStartProperty() == propertyItem)
                    makePropertyLinkBidirectional(propertyLink, true);
            }
        }

        e->accept();
    }
    else
        QGraphicsScene::mouseDoubleClickEvent(e);
}

void LinkDialogGraphicsScene::addPropertyLink(Property* sProp, Property* eProp, bool bidirectional) {
    LinkDialogPropertyGraphicsItem* startProperty =  qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemAt(sProp));
    LinkDialogPropertyGraphicsItem* endProperty =  qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemAt(eProp));
    addPropertyLink(startProperty, endProperty);
    DialogConnectionGraphicsItem* propertyLinkItem = getConnectionGraphicsItem(startProperty, endProperty);

    if (bidirectional)
        makePropertyLinkBidirectional(propertyLinkItem, true);

    addConnectionToCurrentList(propertyLinkItem);
}

int LinkDialogGraphicsScene::currentLinkItemsCount() {
    return static_cast<int>(currentConnectionGraphicsItems_.size());
}

void LinkDialogGraphicsScene::addPropertyLink(PropertyLink* propertyLink) {
    //For adding representations for existing links in the network
    //LogInfo("Adding Property Link.");
    LinkDialogPropertyGraphicsItem* startProperty =  qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemAt(
                propertyLink->getSourceProperty()));
    LinkDialogPropertyGraphicsItem* endProperty =  qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemAt(
                propertyLink->getDestinationProperty()));
    initializePorpertyLinkRepresentation(startProperty, endProperty, propertyLink);
}

void LinkDialogGraphicsScene::addPropertyLink(LinkDialogPropertyGraphicsItem* startProperty, LinkDialogPropertyGraphicsItem* endProperty) {
    //For adding new links to network and creating representations
    //LogInfo("Adding Property Link.");
    Property* sProp = startProperty->getGraphicsItemData();
    Property* eProp = endProperty->getGraphicsItemData();
    Processor* startProcessor = dynamic_cast<Processor*>(sProp->getOwner());
    Processor* endProcessor = dynamic_cast<Processor*>(eProp->getOwner());
    ProcessorLink* processorLink = processorNetwork_->getLink(startProcessor, endProcessor);

    if (!processorLink->isLinked(sProp, eProp) && !processorLink->isLinked(eProp, sProp)) {
        processorLink->addPropertyLinks(sProp, eProp);
        PropertyLink* propertyLink = processorLink->getPropertyLink(sProp, eProp);

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
    Processor* startProcessor = dynamic_cast<Processor*>(startProperty->getGraphicsItemData()->getOwner());
    Processor* endProcessor   = dynamic_cast<Processor*>(endProperty->getGraphicsItemData()->getOwner());
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
    Processor* startProcessor = dynamic_cast<Processor*>(startProperty->getGraphicsItemData()->getOwner());
    Processor* endProcessor   = dynamic_cast<Processor*>(endProperty->getGraphicsItemData()->getOwner());
    ProcessorLink* processorLink = processorNetwork_->getLink(startProcessor, endProcessor);
    PropertyLink* propLink = processorLink->getPropertyLink(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());
    return processorLink->getBidirectionalPair(propLink)!=0;
}

void LinkDialogGraphicsScene::makePropertyLinkBidirectional(DialogConnectionGraphicsItem* propertyLink, bool isBidirectional) {
    LinkDialogPropertyGraphicsItem* startProperty = propertyLink->getStartProperty();
    LinkDialogPropertyGraphicsItem* endProperty = propertyLink->getEndProperty();
    Processor* startProcessor = dynamic_cast<Processor*>(startProperty->getGraphicsItemData()->getOwner());
    Processor* endProcessor   = dynamic_cast<Processor*>(endProperty->getGraphicsItemData()->getOwner());
    ProcessorLink* processorLink = processorNetwork_->getLink(startProcessor, endProcessor);
    PropertyLink* propLink = processorLink->getPropertyLink(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());

    if (isBidirectional) {
        if (processorLink->getBidirectionalPair(propLink)==0)
            processorLink->addPropertyLinks(endProperty->getGraphicsItemData(), startProperty->getGraphicsItemData());
    }
    else {
        if (processorLink->getBidirectionalPair(propLink))
            processorLink->removeBidirectionalPair(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());
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
    Processor* startProcessor = dynamic_cast<Processor*>(outProperty->getGraphicsItemData()->getOwner());
    Processor* endProcessor   = dynamic_cast<Processor*>(inProperty->getGraphicsItemData()->getOwner());
    ProcessorLink* processorLink = processorNetwork_->getLink(startProcessor, endProcessor);
    DialogConnectionGraphicsItem* cItem = new DialogConnectionGraphicsItem(outProperty, inProperty, propertyLink, processorLink);
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

    if (linkGraphicsItem) {
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

    std::vector<ProcessorLink*> processorLinks = processorNetwork_->getLinks();

    for (size_t i=0; i<processorLinks.size(); i++) {
        Processor* srcProcessor = processorLinks[i]->getSourceProcessor();
        Processor* dstProcessor = processorLinks[i]->getDestinationProcessor();

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
}

void LinkDialogGraphicsScene::addProcessorsItemsToScene(Processor* prcoessor, int xPosition, int yPosition) {
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
    procGraphicsItem->setProcessor(prcoessor);
    addItem(procGraphicsItem);
    procGraphicsItem->show();
    std::vector<LinkDialogPropertyGraphicsItem*> propItems = procGraphicsItem->getPropertyItemList();

    for (size_t i=0; i<propItems.size(); i++) {
        addItem(propItems[i]);
        propItems[i]->show();
        propItems[i]->updatePositionBasedOnProcessor();
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

LinkDialog::LinkDialog(std::vector<ProcessorLink*> processorLinks, ProcessorNetwork* network, QWidget* parent) : QDialog(parent) {
    IVW_UNUSED_PARAM(processorLinks);
    IVW_UNUSED_PARAM(parent);
    IVW_UNUSED_PARAM(network);
    //Handle multiple processor links here
}

LinkDialog::LinkDialog(Processor* src, Processor* dest, ProcessorNetwork* network, QWidget* parent) :QDialog(parent) {
    std::vector<Processor*> srcList, dstList;
    srcList.push_back(src);
    dstList.push_back(dest);
    initDialog();
    linkDialogScene_->setNetwork(network); //Network is required to add property links created in dialog (or remove )
    linkDialogScene_->initScene(srcList, dstList);
    src_ = src;
    dest_ = dest;
}

void LinkDialog::initDialog() {
    setWindowTitle("Edit Processor Link Dialog");
    QSize rSize(linkDialogWidth, linkDialogHeight);
    setFixedSize(rSize);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    linkDialogView_ = new LinkDialogGraphicsView(this);
    linkDialogScene_ = new LinkDialogGraphicsScene(this);
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
    autoLinkOptions_ = new CheckableQComboBox("AutoLink Filter", options);
    autoLinkPushButtonLayout->addWidget(autoLinkOptions_, 20);
    //delete button
    deleteAllLinkPushButton_ = new QPushButton("Delete All", this);
    connect(deleteAllLinkPushButton_, SIGNAL(clicked()), this, SLOT(clickedDeleteAllLinksPushButton()));
    autoLinkPushButtonLayout->addWidget(deleteAllLinkPushButton_, 10);
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
}

void LinkDialog::clickedOkayButton() {
    if (linkDialogScene_->currentLinkItemsCount()) {
        ProcessorLink* link = linkDialogScene_->getNetwork()->getLink(src_, dest_);

        if (link)
            link->setSourceModified();
    }

    accept();
}

void LinkDialog::clickedCancelButton() {
    linkDialogScene_->removeCurrentPropertyLinks();
    accept();
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
            if (AutoLinker::canLink(srcProperties[i], dstProperties[j], (LinkingConditions) selectedTypes))
                linkDialogScene_->addPropertyLink(srcProperties[i], dstProperties[j], true);
        }
    }
}

void LinkDialog::clickedDeleteAllLinksPushButton() {
    linkDialogScene_->removeAllPropertyLinks();
}


CheckableQComboBox::CheckableQComboBox(std::string widgetName, std::vector<std::string> options) : QComboBox(),widgetName_(widgetName) {
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