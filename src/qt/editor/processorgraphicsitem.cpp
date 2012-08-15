#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "inviwo/core/ports/port.h"

#include "inviwo/qt/editor/networkeditor.h"
#include "inviwo/qt/editor/connectiongraphicsitem.h"
#include "inviwo/qt/editor/processorgraphicsitem.h"

namespace inviwo {

static const int width = 120;
static const int height = 60;
static const int roundedCorners = 10;
static const int labelHeight = 8;

ProcessorGraphicsItem::ProcessorGraphicsItem()
    : processor_(0) {
    setZValue(2.0f);
    setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable | ItemSendsGeometryChanges);
    setRect(-width/2, -height/2, width, height);
}

ProcessorGraphicsItem::~ProcessorGraphicsItem() {}

QRectF ProcessorGraphicsItem::calculatePortRect(unsigned int curPort, Port::PortDirection portDir) const {
    QPointF portDims(10.0f, 10.0f);
    if (portDir == Port::INPORT)
        return QRectF(rect().left()+10.0f+curPort*(portDims.x()*1.5), rect().top(),
                      portDims.x(), portDims.y());
    else
        return QRectF(rect().left()+10.0f+curPort*(portDims.x()*1.5), rect().bottom()-portDims.y(),
                      portDims.x(), portDims.y());
}

QRectF ProcessorGraphicsItem::calculatePortRect(Port* port) const {
    if (!port->isOutport()) {
        std::vector<Port*> inports = processor_->getInports();
        for (size_t i=0; i<inports.size(); i++)
            if (inports[i] == port)
                return calculatePortRect(i, Port::INPORT);
    } else {
        std::vector<Port*> outports = processor_->getOutports();
        for (size_t i=0; i<outports.size(); i++)
            if (outports[i] == port)
                return calculatePortRect(i, Port::OUTPORT);
    }
    return QRectF();
}

Port* ProcessorGraphicsItem::getSelectedPort(const QPointF pos) const {
    QPointF itemPos = mapFromScene(pos);

    std::vector<Port*> inports = processor_->getInports();
    for (size_t i=0; i<inports.size(); i++) {
        QRectF portRect = calculatePortRect(i, Port::INPORT);
        if (portRect.contains(itemPos))
            return inports[i];
    }

    std::vector<Port*> outports = processor_->getOutports();
    for (size_t i=0; i<outports.size(); i++) {
        QRectF portRect = calculatePortRect(i, Port::OUTPORT);
        if (portRect.contains(itemPos))
            return outports[i];
    }
    return 0;
}

void ProcessorGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    IVW_UNUSED_PARAM(widget);

    p->save();
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
    roundRectPath.moveTo(bRect.left(), bRect.top()+roundedCorners);
    roundRectPath.lineTo(bRect.left(), bRect.bottom()-roundedCorners);
    roundRectPath.arcTo(bRect.left(), bRect.bottom()-(2*roundedCorners), (2*roundedCorners), (2*roundedCorners), 180.0, 90.0);
    roundRectPath.lineTo(bRect.right()-roundedCorners, bRect.bottom());
    roundRectPath.arcTo(bRect.right()-(2*roundedCorners), bRect.bottom()-(2*roundedCorners), (2*roundedCorners), (2*roundedCorners), 270.0, 90.0);
    roundRectPath.lineTo(bRect.right(), bRect.top()+roundedCorners);
    roundRectPath.arcTo(bRect.right()-(2*roundedCorners), bRect.top(), (2*roundedCorners), (2*roundedCorners), 0.0, 90.0);
    roundRectPath.lineTo(bRect.left()+roundedCorners, bRect.top());
    roundRectPath.arcTo(bRect.left(), bRect.top(), (2*roundedCorners), (2*roundedCorners), 90.0, 90.0);
    p->drawPath(roundRectPath);

    // paint inports
    std::vector<Port*> inports = processor_->getInports();
    for (size_t i=0; i<inports.size(); i++) {
        QRectF portRect = calculatePortRect(i, Port::INPORT);
        ivec3 portColor = inports[i]->getColorCode();
        p->setBrush(QColor(portColor.r, portColor.g, portColor.b));
        p->drawRect(portRect);
    }

    // paint outports
    std::vector<Port*> outports = processor_->getOutports();
    for (size_t i=0; i<outports.size(); i++) {
        QRectF portRect = calculatePortRect(i, Port::OUTPORT);
        ivec3 portColor = outports[i]->getColorCode();
        p->setBrush(QColor(portColor.r, portColor.g, portColor.b));
        p->drawRect(portRect);
    }

    // paint labels
    p->setBrush(Qt::NoBrush);
    QRectF nameRect(rect().left(), rect().top()+rect().height()/2.0f-labelHeight*2.0,
                    rect().width(), labelHeight*2.0);
    QRectF classRect(nameRect.left(), nameRect.bottom(), rect().width(), labelHeight*2.0);
    p->setPen(Qt::white);
    p->setFont(QFont("Segoe", labelHeight, QFont::Black, false));
    p->drawText(nameRect, Qt::AlignCenter, QString::fromStdString(processor_->getIdentifier()));
    p->setPen(Qt::lightGray);
    p->setFont(QFont("Segoe", labelHeight, QFont::Normal, true));
    p->drawText(classRect, Qt::AlignCenter, QString::fromStdString(processor_->getClassName()));

    p->restore();
}

QVariant ProcessorGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if(change == ItemPositionChange) {
        std::vector<ConnectionGraphicsItem*> connectionGraphicsItems = NetworkEditor::instance()->connectionGraphicsItems_;
        for (size_t i=0; i<connectionGraphicsItems.size(); i++) {
            if (connectionGraphicsItems[i]->getOutProcessor() == this) {
                //QLineF oldLine = connectionGraphicsItems[i]->line();
                QPointF newAnchor = mapToScene(calculatePortRect(connectionGraphicsItems[i]->getOutport())).boundingRect().center();
                //connectionGraphicsItems[i]->setLine(newAnchor.x(), newAnchor.y(), oldLine.x2(), oldLine.y2());
                connectionGraphicsItems[i]->setStartPoint(newAnchor);
            }
            if (connectionGraphicsItems[i]->getInProcessor() == this) {
                //QLineF oldLine = connectionGraphicsItems[i]->line();
                QPointF newAnchor = mapToScene(calculatePortRect(connectionGraphicsItems[i]->getInport())).boundingRect().center();
                //connectionGraphicsItems[i]->setLine(oldLine.x1(), oldLine.y1(), newAnchor.x(), newAnchor.y());
                connectionGraphicsItems[i]->setEndPoint(newAnchor);
            }
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

} // namespace
