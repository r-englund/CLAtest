#include <QPainter>

#include "inviwo/qt/editor/connectiongraphicsitem.h"

namespace inviwo {

ConnectionGraphicsItem::ConnectionGraphicsItem(ProcessorGraphicsItem* outProcessor, Port* outport,
                                               ProcessorGraphicsItem* inProcessor, Port* inport)
                                               : outProcessor_(outProcessor), outport_(outport),
                                                 inProcessor_(inProcessor), inport_(inport) {
    setZValue(2.0f);
    setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
}

ConnectionGraphicsItem::~ConnectionGraphicsItem() {}

void ConnectionGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    IVW_UNUSED_PARAM(options);
    IVW_UNUSED_PARAM(widget);

    p->setPen(QPen(Qt::darkGray, 4.0f));
    p->drawLine(line().x1(), line().y1(), line().x2(), line().y2());
}

} // namespace