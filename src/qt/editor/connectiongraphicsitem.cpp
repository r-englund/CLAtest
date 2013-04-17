#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>

#include <inviwo/qt/editor/connectiongraphicsitem.h>

namespace inviwo {

CurveGraphicsItem::CurveGraphicsItem(QPointF startPoint, QPointF endPoint, uvec3 color)
                                     : startPoint_(startPoint),
                                       endPoint_(endPoint),
                                       color_(color.r, color.g, color.b) {
    setZValue(CONNECTIONGRAPHICSITEM_DEPTH);

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setOffset(3.0);
    shadowEffect->setBlurRadius(3.0);    
    setGraphicsEffect(shadowEffect);
}

CurveGraphicsItem::~CurveGraphicsItem() {}

QPainterPath CurveGraphicsItem::obtainCurvePath() const {
    float delta = endPoint_.y()-startPoint_.y();
    
    QPointF ctrlPoint1 = QPointF(startPoint_.x(), endPoint_.y()-delta/4.0);
    QPointF ctrlPoint2 = QPointF(endPoint_.x(), startPoint_.y()+delta/4.0);

    if (1==2) {//!verticalLayout_) {
        delta = endPoint_.x()-startPoint_.x();
        ctrlPoint1 = QPointF(startPoint_.x()+delta/4.0, startPoint_.y());
        ctrlPoint2 = QPointF(endPoint_.x()-delta/4.0, endPoint_.y());
    }

    QPainterPath bezierCurve;
    bezierCurve.moveTo(startPoint_);
    bezierCurve.cubicTo(ctrlPoint1, ctrlPoint2, endPoint_);
    return bezierCurve;
}

void CurveGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    IVW_UNUSED_PARAM(options);
    IVW_UNUSED_PARAM(widget);

    if (isSelected()) 
        p->setPen(QPen(Qt::darkRed, 4.0, Qt::SolidLine, Qt::RoundCap));        
    else
        p->setPen(QPen(Qt::black, 3.0, Qt::SolidLine, Qt::RoundCap));

    p->drawPath(obtainCurvePath());

    p->setPen(QPen(color_, 2.0, Qt::SolidLine, Qt::RoundCap));
    p->drawPath(obtainCurvePath());
}

QPainterPath CurveGraphicsItem::shape() const {
    QPainterPathStroker pathStrocker;
    pathStrocker.setWidth(10.0);
    return pathStrocker.createStroke(obtainCurvePath());
}

QRectF CurveGraphicsItem::boundingRect() const {
    QPointF topLeft = QPointF(std::min(startPoint_.x(), endPoint_.x()),
                              std::min(startPoint_.y(), endPoint_.y()));
    return QRectF(topLeft.x()-30.0, topLeft.y()-30.0,
                  abs(startPoint_.x()-endPoint_.x())+60.0,
                  abs(startPoint_.y()-endPoint_.y())+60.0);
}



ConnectionGraphicsItem::ConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor, Port* outport,
                                               ProcessorGraphicsItem* inProcessor, Port* inport)
                                               : CurveGraphicsItem(outProcessor->mapToScene(outProcessor->calculatePortRect(outport)).boundingRect().center(),
                                                                   inProcessor->mapToScene(inProcessor->calculatePortRect(inport)).boundingRect().center(), 
                                                                   inport->getColorCode()),
                                                 outProcessor_(outProcessor), 
                                                 inProcessor_(inProcessor),
                                                 outport_(outport),
                                                 inport_(inport) {
    setFlags(ItemIsSelectable | ItemIsFocusable);
}

ConnectionGraphicsItem::~ConnectionGraphicsItem() {}

} // namespace