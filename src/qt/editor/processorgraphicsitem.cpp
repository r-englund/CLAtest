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
 * Main file author: Timo Ropinski
 *
 *********************************************************************************/

#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QVector2D>

#include <inviwo/core/ports/inport.h>
#include <inviwo/core/ports/outport.h>
#include <inviwo/core/processors/progressbarowner.h>
#include <inviwo/core/metadata/processormetadata.h>

#include <inviwo/qt/editor/networkeditor.h>
#include <inviwo/qt/editor/connectiongraphicsitem.h>
#include <inviwo/qt/editor/linkgraphicsitem.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>
#include <inviwo/qt/widgets/propertylistwidget.h>
#include <inviwo/qt/widgets/processors/processorwidgetqt.h>

namespace inviwo {

static const int width = 150;
static const int height = 50;
static const int roundedCorners = 9;
static const int labelHeight = 8;

int pointSizeToPixelSize(const int pointSize) {
    // compute pixel size for fonts by assuming 96 dpi as basis
    return ((pointSize * 4) / 3);
}

ProcessorGraphicsItem::ProcessorGraphicsItem()
    : ProcessorObserver(), processor_(0) {
    setZValue(PROCESSORGRAPHICSITEM_DEPTH);
    setFlags(ItemIsMovable | ItemIsSelectable | ItemIsFocusable | ItemSendsGeometryChanges);
    setRect(-width/2, -height/2, width, height);
    QGraphicsDropShadowEffect* processorShadowEffect = new QGraphicsDropShadowEffect();
    processorShadowEffect->setOffset(3.0);
    processorShadowEffect->setBlurRadius(3.0);
    setGraphicsEffect(processorShadowEffect);
    nameLabel_ = new LabelGraphicsItem(this);
    nameLabel_->setCrop(9, 8);
    nameLabel_->setPos(-width/2.0+labelHeight, -height/2.0+labelHeight);
    nameLabel_->setDefaultTextColor(Qt::white);
    QFont nameFont("Segoe", labelHeight, QFont::Black, false);
    nameFont.setPixelSize(pointSizeToPixelSize(labelHeight));
    nameLabel_->setFont(nameFont);
    addObservation(nameLabel_);
    classLabel_ = new LabelGraphicsItem(this);
    classLabel_->setCrop(9, 8);
    classLabel_->setPos(-width/2.0+labelHeight, -height/2.0+labelHeight*2.5);
    classLabel_->setDefaultTextColor(Qt::lightGray);
    QFont classFont("Segoe", labelHeight, QFont::Normal, true);
    classFont.setPixelSize(pointSizeToPixelSize(labelHeight));
    classLabel_->setFont(classFont);
    progressBarTimer_.start();
}

ProcessorGraphicsItem::~ProcessorGraphicsItem() {}

void ProcessorGraphicsItem::setProcessor(Processor* processor) {
    processor_ = processor;

    if (processor) {
        nameLabel_->setText(QString::fromStdString(processor_->getIdentifier()));
        classLabel_->setText(QString::fromStdString(processor_->getClassName()));
        addObservation(processor_);
        processor_->addObserver(this);
    } else {
        nameLabel_->setText("");
        classLabel_->setText("");
    }
}

void ProcessorGraphicsItem::editProcessorName() {
    setFocus();
    nameLabel_->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    nameLabel_->setTextInteractionFlags(Qt::TextEditorInteraction);
    nameLabel_->setFocus();
}

QPointF ProcessorGraphicsItem::getShortestBoundaryPointTo(ProcessorGraphicsItem* processorGraphicsItem) {
    return getShortestBoundaryPointTo(processorGraphicsItem->pos());
}

QPointF ProcessorGraphicsItem::getShortestBoundaryPointTo(QPointF inPos) {
    QPointF c = pos();
    QPointF bl = rect().bottomLeft();
    QPointF br = rect().bottomRight();
    QPointF propertyMappedDim ;
    std::vector<QPointF> centerPoints;
    //center
    centerPoints.push_back(c);
    //right boundary center
    propertyMappedDim = mapToParent(br) - mapToParent(bl);
    centerPoints.push_back(c + (propertyMappedDim/2.0));
    //left boundary center
    propertyMappedDim = mapToParent(bl) - mapToParent(br);
    centerPoints.push_back(c + (propertyMappedDim/2.0));
    qreal minDist = std::numeric_limits<qreal>::max();
    size_t minInd=0;

    for (size_t i=1; i<centerPoints.size(); i++) {
        if (QVector2D(centerPoints[i] - inPos).length()<minDist) {
            minInd = i;
            minDist = QVector2D(centerPoints[i] - inPos).length();
        }
    }

    return centerPoints[minInd];
}

QRectF ProcessorGraphicsItem::calculatePortRect(Inport* port) const {
    std::vector<Inport*> inports = processor_->getInports();

    for (size_t i=0; i<inports.size(); i++)
        if (inports[i] == port)
            return calculateInportRect(i);

    return QRectF();
}

QRectF ProcessorGraphicsItem::calculatePortRect(Outport* port) const {
    std::vector<Outport*> outports = processor_->getOutports();

    for (size_t i=0; i<outports.size(); i++)
        if (outports[i] == port)
            return calculateOutportRect(i);

    return QRectF();
}

QRectF ProcessorGraphicsItem::calculateInportRect(size_t curPort) const {
    QPointF portDims(9.0f, 9.0f);
    float xOffset = 8.0f;   // based on roundedCorners
    float xSpacing = 12.5f; // GRID_SIZE / 2.0
    qreal left = rect().left()+xOffset+curPort*xSpacing;
    qreal top = rect().top();
    return QRectF(left, top, portDims.x(), portDims.y());
}

QRectF ProcessorGraphicsItem::calculateOutportRect(size_t curPort) const {
    QPointF portDims(9.0f, 9.0f);
    float xOffset = 8.0f;   // based on roundedCorners
    float xSpacing = 12.5f; // GRID_SIZE / 2.0
    qreal left = rect().left()+xOffset+curPort*xSpacing;
    qreal top = rect().bottom()-portDims.y();
    return QRectF(left, top, portDims.x(), portDims.y());
}

Port* ProcessorGraphicsItem::getSelectedPort(const QPointF pos) const {
    QPointF itemPos = mapFromScene(pos);
    std::vector<Inport*> inports = processor_->getInports();

    for (size_t i=0; i<inports.size(); i++) {
        QRectF portRect = calculateInportRect(i);

        if (portRect.contains(itemPos))
            return inports[i];
    }

    std::vector<Outport*> outports = processor_->getOutports();

    for (size_t i=0; i<outports.size(); i++) {
        QRectF portRect = calculateOutportRect(i);

        if (portRect.contains(itemPos))
            return outports[i];
    }

    return 0;
}

void ProcessorGraphicsItem::paintStatusIndicator(QPainter* p, QPointF pos,
        bool isOn, QColor baseColor) {

    qreal ledRadius = 5.0;
    QColor ledColor;
    QColor borderColor;
    if (isOn) {
        ledColor = baseColor;
        borderColor = QColor(124,124,124);
        //borderColor = baseColor.dark(110);//QColor(64,64,64);
    } else {
        ledColor = baseColor.dark(400);
        borderColor = QColor(64,64,64);
    }

    // initialize painter
    p->save();
    p->setPen(QPen(borderColor, 3.0));
    p->setBrush(QBrush(ledColor));
    // draw base shape
    p->drawEllipse(pos, ledRadius, ledRadius);
    // draw light highlight
    QPointF highLightPos = pos;
    p->setPen(Qt::NoPen);
    while (ledRadius > 0.0) {
        ledColor = ledColor.light(120);
        p->setBrush(QBrush(ledColor));
        p->drawEllipse(highLightPos, ledRadius, ledRadius);
        ledRadius -= 0.25;
        p->drawEllipse(highLightPos, ledRadius, ledRadius);
        ledRadius -= 0.25;        
        p->drawEllipse(highLightPos, ledRadius, ledRadius);
        ledRadius -= 0.25;        
        highLightPos.setX(highLightPos.x()-0.25);
        highLightPos.setY(highLightPos.y()-0.25);
    }
    // deinitialize painter
    p->restore();
}

void ProcessorGraphicsItem::paintProgressBar(QPainter* p, float progress) {
    QPointF position(-(width/2.0f)+7.0f, 9.0f);
    QSize dimensions(width-14.0f, 5.0f);
    p->save();
    QColor progressColor = Qt::darkGray;
    QRectF progressBarRect(position, dimensions);
    QLinearGradient progressGrad(progressBarRect.topLeft(), progressBarRect.topRight());
    progressGrad.setColorAt(0.0f, progressColor);
    float left = std::max(0.0f, progress-0.001f);
    float right = std::min(1.0f, progress+0.001f);
    progressGrad.setColorAt(left, progressColor);
    progressGrad.setColorAt(right, Qt::black);
    progressGrad.setColorAt(1.0f, Qt::black);
    p->setPen(Qt::black);
    p->setBrush(progressGrad);
    p->drawRoundedRect(progressBarRect, 2.0, 2.0);
    QColor shadeColor(128,128,128);
    QLinearGradient shadingGrad(progressBarRect.topLeft(), progressBarRect.bottomLeft());
    shadingGrad.setColorAt(0.0f, QColor(shadeColor.red()*0.6, shadeColor.green()*0.6, shadeColor.blue()*0.6, 120));
    shadingGrad.setColorAt(0.3f, QColor(shadeColor.red(), shadeColor.green(), shadeColor.blue(), 120));
    shadingGrad.setColorAt(1.0f, QColor(shadeColor.red(), shadeColor.green(), shadeColor.blue(), 120));
    p->setPen(Qt::NoPen);
    p->setBrush(shadingGrad);
    p->drawRoundedRect(progressBarRect, 2.0, 2.0);
    p->restore();
}

void ProcessorGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
    IVW_UNUSED_PARAM(options);
    IVW_UNUSED_PARAM(widget);
    p->save();
    p->setPen(Qt::NoPen);
    p->setRenderHint(QPainter::Antialiasing, true);
    QColor topColor(140,140,140);
    QColor middleColor(59,61,61);
    QColor bottomColor(40,40,40);
    // paint processor
    QLinearGradient grad(rect().topLeft(), rect().bottomLeft());

    if (isSelected()) {
        grad.setColorAt(0.0f, topColor);
        grad.setColorAt(0.2f, middleColor);
        grad.setColorAt(0.5f, Qt::darkRed);
        grad.setColorAt(1.0f, bottomColor);
    } else {
        grad.setColorAt(0.0f, topColor);
        grad.setColorAt(0.2f, middleColor);
        grad.setColorAt(1.0f, bottomColor);
    }

    QRectF bRect = rect();
    QPainterPath roundRectPath;
    roundRectPath.moveTo(bRect.left(), bRect.top()+roundedCorners);
    roundRectPath.lineTo(bRect.left(), bRect.bottom()-roundedCorners);
    roundRectPath.arcTo(bRect.left(), bRect.bottom()-(2*roundedCorners), (2*roundedCorners), (2*roundedCorners), 180.0, 90.0);
    roundRectPath.lineTo(bRect.right()-roundedCorners, bRect.bottom());
    roundRectPath.arcTo(bRect.right()-(2*roundedCorners), bRect.bottom()-(2*roundedCorners), (2*roundedCorners), (2*roundedCorners), 270.0,
                        90.0);
    roundRectPath.lineTo(bRect.right(), bRect.top()+roundedCorners);
    roundRectPath.arcTo(bRect.right()-(2*roundedCorners), bRect.top(), (2*roundedCorners), (2*roundedCorners), 0.0, 90.0);
    roundRectPath.lineTo(bRect.left()+roundedCorners, bRect.top());
    roundRectPath.arcTo(bRect.left(), bRect.top(), (2*roundedCorners), (2*roundedCorners), 90.0, 90.0);
    p->setBrush(grad);
    p->drawPath(roundRectPath);
    QLinearGradient highlightGrad(rect().topLeft(), rect().bottomLeft());

    if (isSelected()) {
        highlightGrad.setColorAt(0.0f, bottomColor);
        highlightGrad.setColorAt(0.1f, bottomColor);
        highlightGrad.setColorAt(0.5f, Qt::darkRed);
        highlightGrad.setColorAt(1.0f, bottomColor);
    } else {
        highlightGrad.setColorAt(0.0f, bottomColor);
        highlightGrad.setColorAt(1.0f, bottomColor);
    }

    QPainterPath highlightPath;
    float highlightLength = bRect.width()/8.0;
    highlightPath.moveTo(bRect.left(), bRect.top()+roundedCorners);
    highlightPath.lineTo(bRect.left(), bRect.bottom()-roundedCorners);
    highlightPath.arcTo(bRect.left(), bRect.bottom()-(2*roundedCorners), (2*roundedCorners), (2*roundedCorners), 180.0, 90.0);
    highlightPath.lineTo(bRect.left()+(bRect.width()/2.0)+highlightLength, bRect.bottom());
    highlightPath.lineTo(bRect.left()+(bRect.width()/2.0)-highlightLength, bRect.top());
    highlightPath.lineTo(bRect.left()+roundedCorners, bRect.top());
    highlightPath.arcTo(bRect.left(), bRect.top(), (2*roundedCorners), (2*roundedCorners), 90.0, 90.0);
    p->setBrush(highlightGrad);
    p->drawPath(highlightPath);
    p->setPen(QPen(QColor(164,164,164), 1.0));
    p->setBrush(Qt::NoBrush);
    p->drawPath(roundRectPath);
    // paint inports
    p->setPen(QPen(bottomColor, 1.0));
    std::vector<Inport*> inports = processor_->getInports();

    for (size_t i=0; i<inports.size(); i++) {
        QRectF portRect = calculateInportRect(i);
        uvec3 portColor = inports[i]->getColorCode();
        QLinearGradient portGrad(portRect.topLeft(), portRect.bottomLeft());
        portGrad.setColorAt(0.0f, QColor(portColor.r*0.6, portColor.g*0.6, portColor.b*0.6));
        portGrad.setColorAt(0.3f, QColor(portColor.r, portColor.g, portColor.b));
        portGrad.setColorAt(1.0f, QColor(portColor.r, portColor.g, portColor.b));
        p->setBrush(portGrad);
        p->drawRect(portRect);
    }

    // paint outports
    std::vector<Outport*> outports = processor_->getOutports();

    for (size_t i=0; i<outports.size(); i++) {
        QRectF portRect = calculateOutportRect(i);
        uvec3 portColor = outports[i]->getColorCode();
        QLinearGradient portGrad(portRect.topLeft(), portRect.bottomLeft());
        portGrad.setColorAt(0.0f, QColor(portColor.r*0.6, portColor.g*0.6, portColor.b*0.6));
        portGrad.setColorAt(0.3f, QColor(portColor.r, portColor.g, portColor.b));
        portGrad.setColorAt(1.0f, QColor(portColor.r, portColor.g, portColor.b));
        p->setBrush(portGrad);
        p->drawRect(portRect);
    }

    paintStatusIndicator(p, QPointF(64.0f, -15.0f),
                         processor_->isReady(), QColor(0,170,0));
    //TODO: Fix progressbar to true indicator or just make a processing wheel.
    /*ProgressBarOwner* progressBarOwner = dynamic_cast<ProgressBarOwner*>(processor_);
    if (progressBarOwner != NULL) {
        paintProgressBar(p, progressBarOwner->getProgressBar().getProgress());
    }*/
    p->restore();
}

bool ProcessorGraphicsItem::isEditingProcessorName() {
    return (nameLabel_->textInteractionFlags() == Qt::TextEditorInteraction);
}

void ProcessorGraphicsItem::setIdentifier(QString text) {
    std::string oldName = getProcessor()->getIdentifier();
    std::string newName = text.toLocal8Bit().constData();
    if(newName.size()==0){
        nameLabel_->setText(oldName.c_str());
        return;
    }
    getProcessor()->setIdentifier(newName);
    ProcessorWidgetQt* processorWidgetQt = dynamic_cast<ProcessorWidgetQt*>(getProcessor()->getProcessorWidget());

    if (processorWidgetQt)
        processorWidgetQt->setWindowTitle(text);

    PropertyListWidget::instance()->changeName(oldName, newName);
}

QVariant ProcessorGraphicsItem::itemChange(GraphicsItemChange change, const QVariant& value) {
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        std::vector<ConnectionGraphicsItem*> connectionGraphicsItems = NetworkEditor::getRef().connectionGraphicsItems_;

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

        std::vector<LinkConnectionGraphicsItem*> linkGraphicsItems = NetworkEditor::getRef().linkGraphicsItems_;

        for (size_t i=0; i<linkGraphicsItems.size(); i++) {
            if (linkGraphicsItems[i]->getSrcProcessorGraphicsItem() == this || linkGraphicsItems[i]->getDestProcessorGraphicsItem() == this) {
                QPointF startPoint = linkGraphicsItems[i]->getSrcProcessorGraphicsItem()->getShortestBoundaryPointTo(
                                         linkGraphicsItems[i]->getDestProcessorGraphicsItem());
                QPointF endPoint = linkGraphicsItems[i]->getDestProcessorGraphicsItem()->getShortestBoundaryPointTo(
                                       linkGraphicsItems[i]->getSrcProcessorGraphicsItem());
                linkGraphicsItems[i]->setStartPoint(startPoint);
                linkGraphicsItems[i]->setEndPoint(endPoint);
                linkGraphicsItems[i]->update();
            }
        }

        updateMetaData();
    }
    else if (change == QGraphicsItem::ItemSelectedHasChanged) {
        if (isSelected()) NetworkEditor::getPtr()->addPropertyWidgets(getProcessor());
        else NetworkEditor::getPtr()->removePropertyWidgets(getProcessor());
    }

    return QGraphicsItem::itemChange(change, value);
}

void ProcessorGraphicsItem::updateMetaData() {
    //TODO: this is not pretty way to access processor metadata, find another way
    ProcessorMetaData* processorMeta = dynamic_cast<ProcessorMetaData*>(processor_->getMetaData("ProcessorMetaData"));
    processorMeta->setVisibile(isVisible());
    processorMeta->setPosition(ivec2(x(), y()));
    processorMeta->setSelected(isSelected());
}

void ProcessorGraphicsItem::notify() {
    if (nameLabel_->isFocusOut()) {
        setIdentifier(nameLabel_->text());
        nameLabel_->setNoFocusOut();
    }
}

} // namespace
