#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>

#include "inviwo/qt/editor/linkgraphicsitem.h"

namespace inviwo {

LinkGraphicsItem::LinkGraphicsItem(QPointF startPoint, QPointF endPoint, bool layoutOption, ivec3 color, bool dragMode)
                                     : startPoint_(startPoint),
                                       endPoint_(endPoint),
                                       verticalLayout_(layoutOption),
                                       color_(color.r, color.g, color.b),
                                       dragMode_(dragMode){
    setZValue(LINKGRAPHICSITEM_DEPTH);

    QGraphicsDropShadowEffect* processorShadowEffect = new QGraphicsDropShadowEffect();
    processorShadowEffect->setOffset(3.0);
    processorShadowEffect->setBlurRadius(3.0);    
    setGraphicsEffect(processorShadowEffect);
}

LinkGraphicsItem::~LinkGraphicsItem() {}

QPainterPath LinkGraphicsItem::obtainCurvePath() const {
    float delta = endPoint_.y()-startPoint_.y();
        
    QPointF ctrlPoint1 = QPointF((startPoint_.x()+endPoint_.x())/2.0, startPoint_.y());
    QPointF ctrlPoint2 = QPointF(endPoint_.x(), startPoint_.y()+delta/2.0);

    if (!verticalLayout_) {
        delta = endPoint_.x()-startPoint_.x();
        ctrlPoint1 = QPointF(startPoint_.x()+delta/4.0, startPoint_.y());
        ctrlPoint2 = QPointF(endPoint_.x()-delta/4.0, endPoint_.y());
    }
    
    QPainterPath bezierCurve;
    bezierCurve.moveTo(startPoint_);
    bezierCurve.cubicTo(ctrlPoint1, ctrlPoint2, endPoint_);
    return bezierCurve;
}

void LinkGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    IVW_UNUSED_PARAM(options);
    IVW_UNUSED_PARAM(widget);

    if(!dragMode_) {
        if (isSelected()) 
            p->setPen(QPen(Qt::darkRed, 3.0, Qt::DotLine, Qt::RoundCap));        
        else
            p->setPen(QPen(Qt::black, 2.0, Qt::DotLine, Qt::RoundCap));

        p->drawPath(obtainCurvePath());
    }

    p->setPen(QPen(color_, 2.0, Qt::DotLine, Qt::RoundCap));
    p->drawPath(obtainCurvePath());
}

QPainterPath LinkGraphicsItem::shape() const {
    QPainterPathStroker pathStrocker;
    pathStrocker.setWidth(10.0);
    return pathStrocker.createStroke(obtainCurvePath());
}

QRectF LinkGraphicsItem::boundingRect() const {
    QPointF topLeft = QPointF(std::min(startPoint_.x(), endPoint_.x()),
                              std::min(startPoint_.y(), endPoint_.y()));
    return QRectF(topLeft.x()-30.0, topLeft.y()-30.0,
                  abs(startPoint_.x()-endPoint_.x())+60.0,
                  abs(startPoint_.y()-endPoint_.y())+60.0);
}



LinkConnectionGraphicsItem::LinkConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor,
                                               ProcessorGraphicsItem* inProcessor, bool layoutOption)
                                               : LinkGraphicsItem(outProcessor->mapToScene(outProcessor->rect()).boundingRect().center(),
                                                                   inProcessor->mapToScene(inProcessor->rect()).boundingRect().center(), 
                                                                   layoutOption, ivec3(256,256,256), false),
                                                 outProcessor_(outProcessor), inProcessor_(inProcessor) {
    setFlags(ItemIsSelectable | ItemIsFocusable);
}

LinkConnectionGraphicsItem::~LinkConnectionGraphicsItem() {}

void LinkConnectionGraphicsItem::flipLayout() {
    setStartPoint(outProcessor_->mapToScene(inProcessor_->rect()).boundingRect().center());
    setEndPoint(inProcessor_->mapToScene(inProcessor_->rect()).boundingRect().center());

    if (getLayoutOption()) {
        setLayoutOption(false);
    }
    else
        setLayoutOption(true);

}

} // namespace