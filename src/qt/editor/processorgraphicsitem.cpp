/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QVector2D>


#include <inviwo/qt/editor/networkeditor.h>
#include <inviwo/qt/editor/connectiongraphicsitem.h>
#include <inviwo/qt/editor/linkgraphicsitem.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>
#include <inviwo/qt/widgets/processors/processorwidgetqt.h>

#include <inviwo/core/util/msvc-memleak-includes.h>

#include <inviwo/core/ports/inport.h>
#include <inviwo/core/ports/outport.h>
#include <inviwo/core/processors/progressbarowner.h>
#include <inviwo/core/metadata/processormetadata.h>

namespace inviwo {

static const int width = 150;
static const int height = 50;
static const int roundedCorners = 9;
static const int labelHeight = 8;

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
    nameLabel_->setCrop(8,7);
    nameLabel_->setPos(-width/2.0+labelHeight, -height/2.0+labelHeight);
    nameLabel_->setDefaultTextColor(Qt::white);
    nameLabel_->setFont(QFont("Segoe", labelHeight, QFont::Black, false));
    addObservation(nameLabel_);

    classLabel_ = new LabelGraphicsItem(this);
    classLabel_->setCrop(8,7);
    classLabel_->setPos(-width/2.0+labelHeight, -height/2.0+labelHeight*2.5);
    classLabel_->setDefaultTextColor(Qt::lightGray);
    classLabel_->setFont(QFont("Segoe", labelHeight, QFont::Normal, true));

    progressBarTimer_.start();
}

ProcessorGraphicsItem::~ProcessorGraphicsItem() {
}

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
    QPointF tr = rect().topRight();
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

    //top boundary center
   // propertyMappedDim = mapToParent(tr) - mapToParent(br);
   // centerPoints.push_back(c + (propertyMappedDim/2.0));

    //bottom boundary center
   // propertyMappedDim = mapToParent(br) - mapToParent(tr);
   // centerPoints.push_back(c + (propertyMappedDim/2.0));
    
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

void ProcessorGraphicsItem::paintStatusIndicator(QPainter* p, QPointF offset,
                                                 bool isOn, QColor baseColor) {
    // This code has been modified based on the KLed class from the KDE libraries
    // TODO: check license implications
    p->save();
    p->translate(offset);

    int width = 12;
    int scale = 1;
    width *= scale;

    QColor color;
    if (isOn) color = baseColor;
    else color = baseColor.dark(500);
     
    p->setBrush(QBrush(color));
    p->drawEllipse(scale, scale, width-scale*2, width-scale*2);

    // draw the bright light spot of the LED
    // set new width of the pen avoid a "pixelized" shadow
     
    // shrink the light on the LED to a size about 3/4 of the complete LED
    int pos = width / 5 + 1;
    int lightWidth = width*3/4;
     
    // Calculate the LED's "light factor"
    int lightFactor = (130*2 / (lightWidth ? lightWidth : 1)) + 100;

    // draw the bright spot on the LED
    while (lightWidth) {
        color = color.light(lightFactor);
        p->setPen(QPen(color));
        p->drawEllipse(pos, pos, lightWidth, lightWidth);
        lightWidth--;
        if (!lightWidth) break;

        p->drawEllipse(pos, pos, lightWidth, lightWidth);
        lightWidth--;
        if (!lightWidth) break;

        p->drawEllipse(pos, pos, lightWidth, lightWidth);
        lightWidth--;
        pos++;
    }

    // draw a thin border around the LED which resembles a shadow with light coming
    // from the upper left
    QPen pen;
    pen.setWidth(2 * scale + 1);
    p->setBrush(Qt::NoBrush);

    // set the initial color and draw the shadow border at 45° (45*16 = 720).
    int angle = -720;
    if (isOn == true) color = baseColor.lighter(100);
    else color = Qt::darkGray;

    for (int arc=120; arc<2880; arc+=240) {
        pen.setColor(color);
        p->setPen(pen);
        int w = width-pen.width()/2 - scale+1;
        p->drawArc(pen.width()/2, pen.width()/2, w, w, angle+arc, 240);
        p->drawArc(pen.width()/2, pen.width()/2, w, w, angle-arc, 240);
        color = color.dark(110);
    }

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
    roundRectPath.arcTo(bRect.right()-(2*roundedCorners), bRect.bottom()-(2*roundedCorners), (2*roundedCorners), (2*roundedCorners), 270.0, 90.0);
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

    paintStatusIndicator(p, QPointF(57.0f, -20.0f),
                         processor_->isReady(), QColor(0,170,0));

    //TODO: Fix progressbar to true indicator or just make a processing wheel.
    /*ProgressBarOwner* progressBarOwner = dynamic_cast<ProgressBarOwner*>(processor_);
    if (progressBarOwner != NULL) {
        paintProgressBar(p, progressBarOwner->getProgressBar().getProgress());
    }*/

    p->restore();
}

void ProcessorGraphicsItem::updatePropertyListWidget(){
    NetworkEditor::getRef().updatePropertyListWidget();
}

bool ProcessorGraphicsItem::isEditingProcessorName(){
    return (nameLabel_->textInteractionFlags() == Qt::TextEditorInteraction);
}

void ProcessorGraphicsItem::setIdentifier(QString text){
    getProcessor()->setIdentifier(text.toLocal8Bit().constData());
    ProcessorWidgetQt* processorWidgetQt = dynamic_cast<ProcessorWidgetQt*>(getProcessor()->getProcessorWidget());
    if(processorWidgetQt)
        processorWidgetQt->setWindowTitle(text);
    updatePropertyListWidget();
}

QVariant ProcessorGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value) {
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
             if (linkGraphicsItems[i]->getOutProcessorGraphicsItem() == this || linkGraphicsItems[i]->getInProcessorGraphicsItem() == this) {
                 QPointF startPoint = linkGraphicsItems[i]->getOutProcessorGraphicsItem()->getShortestBoundaryPointTo(linkGraphicsItems[i]->getInProcessorGraphicsItem());
                 QPointF endPoint = linkGraphicsItems[i]->getInProcessorGraphicsItem()->getShortestBoundaryPointTo(linkGraphicsItems[i]->getOutProcessorGraphicsItem());
                 linkGraphicsItems[i]->setStartPoint(startPoint);
                 linkGraphicsItems[i]->setEndPoint(endPoint);
                 linkGraphicsItems[i]->update();
             }
        }

        updateMetaData();
    }
    return QGraphicsItem::itemChange(change, value);
}

void ProcessorGraphicsItem::updateMetaData() {
    //TODO: this is not pretty way to access processor metadata, find another way
    ProcessorMetaData* processorMeta = dynamic_cast<ProcessorMetaData*>(processor_->getMetaData("ProcessorMetaData"));
    processorMeta->setVisibile(isVisible());
    processorMeta->setPosition(ivec2(x(), y()));
}

void ProcessorGraphicsItem::notify() {
    if(nameLabel_->isFocusOut()){
        setIdentifier(nameLabel_->text());
        nameLabel_->setNoFocusOut();
    }

    //TODO: Fix progressbar to true indicator or just make a processing wheel.
    /*ProgressBarOwner* progressBarOwner = dynamic_cast<ProgressBarOwner*>(processor_);
    if (progressBarOwner != NULL) {
        if (progressBarTimer_.elapsed() > 500 ||
            progressBarOwner->getProgressBar().getProgress()==0.0f || progressBarOwner->getProgressBar().getProgress()==1.0f) {
            progressBarTimer_.restart();
            update();
            //QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 100);
        }
    }*/
}

} // namespace
