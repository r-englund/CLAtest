
#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include "inviwo/qt/editor/processorgraphicsitem.h"
#include "inviwo/core/ports/port.h"

namespace inviwo {

    ProcessorGraphicsItem::ProcessorGraphicsItem() {
        static int count = 0;
        setRect(-150, -50, 300, 100);
        setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable | ItemSendsGeometryChanges);
        setZValue(1.0f);

        processor_ = new Processor();
    }

    ProcessorGraphicsItem::~ProcessorGraphicsItem() {}

    Processor* ProcessorGraphicsItem::getProcessor() const {
        return processor_;
    }

    std::string ProcessorGraphicsItem::getIdentifier() const {
        return processor_->getIdentifier();
    }

    void ProcessorGraphicsItem::setProcessor(Processor* processor) {
        processor_ = processor;
    }

    QRectF ProcessorGraphicsItem::calculatePortRect(unsigned int curPort, Port::PortDirection portDir) {
        QPointF portDims(20.0f, 20.0f);
        if (portDir == Port::INPORT)
            return QRectF(rect().left()+10.0f+curPort*(portDims.x()*1.5), rect().top(), portDims.x(), portDims.y());
        else
            return QRectF(rect().left()+10.0f+curPort*(portDims.x()*1.5), rect().bottom()-portDims.y(), portDims.x(), portDims.y());
    }

    QRectF ProcessorGraphicsItem::calculatePortRect(Port* port) {
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

    Port* ProcessorGraphicsItem::getSelectedPort(const QPointF pos) {
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

    void ProcessorGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* /*w*/) {
        // paint processor
        QLinearGradient grad(rect().topLeft(), rect().bottomLeft());
        if (isSelected()) {
            grad.setColorAt(0.0f, opt->palette.mid().color());
            grad.setColorAt(0.5f, opt->palette.button().color());
            grad.setColorAt(1.0f, opt->palette.mid().color());
        } else {
            grad.setColorAt(0.0f, opt->palette.light().color());
            grad.setColorAt(0.5f, opt->palette.button().color());
            grad.setColorAt(1.0f, opt->palette.light().color());
        }
        p->fillRect(rect(), grad);
        p->drawRect(rect());

        // paint inports
        std::vector<Port*> inports = processor_->getInports();
        for (size_t i=0; i<inports.size(); i++) {
            QRectF portRect = calculatePortRect(i, Port::INPORT);
            p->fillRect(portRect, opt->palette.dark());
            p->drawRect(portRect);
        }

        // paint outports
        std::vector<Port*> outports = processor_->getOutports();
        for (size_t i=0; i<outports.size(); i++) {
            QRectF portRect = calculatePortRect(i, Port::OUTPORT);
            p->fillRect(portRect, opt->palette.dark());
            p->drawRect(portRect);
        }

        // paint labels
        QRectF nameRect(rect().left(), rect().top(), rect().width(), rect().height()/2.0f);
        QRectF classRect(nameRect.left(), nameRect.bottom(), nameRect.width(), nameRect.height());
        p->drawText(nameRect, Qt::AlignCenter, QString::fromStdString(processor_->getIdentifier()));
        p->drawText(classRect, Qt::AlignCenter, QString::fromStdString(processor_->getClassName()));
    }

    QVariant ProcessorGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value) {
        if(change == ItemPositionChange) {
            std::vector<ConnectionGraphicsItem*> connectionGraphicsItems = NetworkEditor::instance()->connectionGraphicsItems_;
            for (size_t i=0; i<connectionGraphicsItems.size(); i++) {
                if (connectionGraphicsItems[i]->getOutProcessor() == this) {
                    QLineF oldLine = connectionGraphicsItems[i]->line();
                    QPointF newAnchor = mapToScene(calculatePortRect(connectionGraphicsItems[i]->getOutport())).boundingRect().center();
                    connectionGraphicsItems[i]->setLine(newAnchor.x(), newAnchor.y(), oldLine.x2(), oldLine.y2());
                }
                if (connectionGraphicsItems[i]->getInProcessor() == this) {
                    QLineF oldLine = connectionGraphicsItems[i]->line();
                    QPointF newAnchor = mapToScene(calculatePortRect(connectionGraphicsItems[i]->getInport())).boundingRect().center();
                    connectionGraphicsItems[i]->setLine(oldLine.x1(), oldLine.y1(), newAnchor.x(), newAnchor.y());
                }
            }
        }
        return QGraphicsItem::itemChange(change, value);
    }

} // namespace
