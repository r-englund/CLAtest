#include "inviwo/qt/editor/connectiongraphicsitem.h"
#include "inviwo/qt/editor/processorgraphicsitem.h"

#include <QStyleOptionGraphicsItem>
#include <QPainter>

namespace inviwo {

ConnectionGraphicsItem::ConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor, Port* outport,
                                               ProcessorGraphicsItem* inProcessor, Port* inport)
                                                : outProcessor_(outProcessor), outport_(outport),
                                                  inProcessor_(inProcessor), inport_(inport) {
   setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
   setZValue(1.0f);
}

ConnectionGraphicsItem::~ConnectionGraphicsItem() {}

void ConnectionGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* w) {
    p->setPen(QPen(Qt::red, 5.0f));
    p->drawLine(line().x1(), line().y1(), line().x2(), line().y2());
}

} // namespace