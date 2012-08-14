#include <QPainter>

#include <QPainterPath>

#include "inviwo/qt/editor/connectiongraphicsitem.h"

namespace inviwo {

ConnectionGraphicsItem::ConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor, Port* outport,
                                               ProcessorGraphicsItem* inProcessor, Port* inport)
                                               : outProcessor_(outProcessor), outport_(outport),
                                                 inProcessor_(inProcessor), inport_(inport) {
    setZValue(1.0f);
    setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
}

ConnectionGraphicsItem::~ConnectionGraphicsItem() {}

void ConnectionGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    IVW_UNUSED_PARAM(options);
    IVW_UNUSED_PARAM(widget);

    p->setPen(QPen(Qt::darkGray, 2.0f));
    QPointF startPoint = QPointF(line().x1(), line().y1());
    QPointF endPoint = QPointF(line().x2(), line().y2());
    float deltaY = endPoint.y()-startPoint.y();
    QPointF ctrlPoint1 = QPointF(startPoint.x(), endPoint.y()-deltaY/4.0);
    QPointF ctrlPoint2 = QPointF(endPoint.x(), startPoint.y()+deltaY/4.0);
    QPainterPath bezierPath;
    bezierPath.moveTo(startPoint);
    bezierPath.cubicTo(ctrlPoint1, ctrlPoint2, endPoint);
    p->drawPath(bezierPath);
}

} // namespace