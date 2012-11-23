#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "inviwo/core/ports/port.h"
#include "inviwo/core/metadata/positionmetadata.h"

#include "inviwo/qt/editor/networkeditor.h"
#include "inviwo/qt/editor/connectiongraphicsitem.h"
#include "inviwo/qt/editor/processorgraphicsitem.h"

namespace inviwo {

static const int width = 120;
static const int height = 60;
static const int roundedCorners = 10;
static const int labelHeight = 8;

ProcessorGraphicsItem::ProcessorGraphicsItem(bool fitVerticalLayout)
    : processor_(0),
      fitVerticalLayout_(fitVerticalLayout) {
    setZValue(PROCESSORGRAPHICSITEM_DEPTH);
    setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable);
    setRect(-width/2, -height/2, width, height);

    QGraphicsDropShadowEffect* processorShadowEffect = new QGraphicsDropShadowEffect();
    processorShadowEffect->setOffset(3.0);
    processorShadowEffect->setBlurRadius(3.0);    
    setGraphicsEffect(processorShadowEffect);

    nameLabel_ = new LabelGraphicsItem(this);
    nameLabel_->setPos(-width/2.0+labelHeight/2.0, -height/2.0+labelHeight);
    nameLabel_->setDefaultTextColor(Qt::white);
    nameLabel_->setFont(QFont("Segoe", labelHeight, QFont::Black, false));
    //nameLabel_->setTextInteractionFlags(Qt::TextEditable);

    classLabel_ = new LabelGraphicsItem(this);
    classLabel_->setPos(-width/2.0+labelHeight/2.0, -height/2.0+labelHeight*2.5);
    classLabel_->setDefaultTextColor(Qt::lightGray);
    classLabel_->setFont(QFont("Segoe", labelHeight, QFont::Normal, true));
}

ProcessorGraphicsItem::~ProcessorGraphicsItem() {
    delete nameLabel_;
}

void ProcessorGraphicsItem::setProcessor(Processor* processor) {
    processor_ = processor;
    if (processor) {
        nameLabel_->setPlainText(QString::fromStdString(processor_->getIdentifier()));
        classLabel_->setPlainText(QString::fromStdString(processor_->getClassName()));
    } else {
        nameLabel_->setPlainText("");
        classLabel_->setPlainText("");
    }
}

void ProcessorGraphicsItem::flipLayout() {
    fitVerticalLayout_ = (fitVerticalLayout_)?false:true;
    //TODO: transform from vertical to horizontal or vice versa
    //if (!fitVerticalLayout_) {        
        vec2 position(x(), y());
        setX(position.y);
        setY(position.x);
        //repaint if necessary
    //}
}

void ProcessorGraphicsItem::editProcessorName() {
    nameLabel_ ->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    nameLabel_ ->setTextInteractionFlags(Qt::TextEditorInteraction);
    nameLabel_->setFocus();
}

QRectF ProcessorGraphicsItem::calculatePortRect(unsigned int curPort, Port::PortDirection portDir) const {
    QPointF portDims(10.0f, 10.0f);
    
    if (portDir == Port::INPORT) {
        qreal left = rect().left()+10.0f+curPort*(portDims.x()*1.5);
        qreal top = rect().top();
        if (!fitVerticalLayout_) {
            left = rect().left();
            top = rect().bottom()-20.0f-curPort*(portDims.y()*1.5);
        }
        return QRectF(left, top, portDims.x(), portDims.y());
    }
    else {
        qreal left = rect().left()+10.0f+curPort*(portDims.x()*1.5);
        qreal top = rect().bottom()-portDims.y();
        if (!fitVerticalLayout_) {
            left = rect().right()-portDims.x();
            top = rect().bottom()-20.0f-curPort*(portDims.y()*1.5);
        }
        return QRectF(left, top, portDims.x(), portDims.y());
    }
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

    p->restore();
}

QVariant ProcessorGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange) {
        std::vector<ConnectionGraphicsItem*> connectionGraphicsItems = NetworkEditor::instance()->connectionGraphicsItems_;
        for (size_t i=0; i<connectionGraphicsItems.size(); i++) {
            if (connectionGraphicsItems[i]->getOutProcessor() == this) {
                QPointF newAnchor = mapToScene(calculatePortRect(connectionGraphicsItems[i]->getOutport())).boundingRect().center();
                connectionGraphicsItems[i]->setStartPoint(newAnchor);
                connectionGraphicsItems[i]->update();
            }
            if (connectionGraphicsItems[i]->getInProcessor() == this) {
                QPointF newAnchor = mapToScene(calculatePortRect(connectionGraphicsItems[i]->getInport())).boundingRect().center();
                connectionGraphicsItems[i]->setEndPoint(newAnchor);
                connectionGraphicsItems[i]->update();
            }
        }
        updateMetaData();
    }
    return QGraphicsItem::itemChange(change, value);
}

void ProcessorGraphicsItem::updateMetaData() {
    //TODO: this is not pretty way to access processor metadata, find another way
    PositionMetaData* position_meta = dynamic_cast<PositionMetaData*>(processor_->getMetaData("PositionMetaData"));
    position_meta->setXY(ivec2(x(), y()));
}

} // namespace
