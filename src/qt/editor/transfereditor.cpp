#include <inviwo/core/inviwoapplication.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/io/serialization/ivwserializer.h>
#include <inviwo/core/io/serialization/ivwdeserializer.h>
#include <inviwo/qt/editor/processorlistwidget.h>
#include <inviwo/qt/editor/propertylistwidget.h>
#include <inviwo/qt/widgets/processors/processorwidgetfactoryqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
#include <inviwo/core/metadata/positionmetadata.h>
#include <inviwo/core/util/inviwofactorybase.h>
#include <inviwo/qt/editor/transfereditor.h>

#include <QApplication>
#include <QBrush>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <QLineF>
#include <QGraphicsLineItem>

#include <QVarLengthArray>


namespace inviwo {

    const std::string TransferEditor::logSource_ = "TransferEditor";

    TransferEditor::TransferEditor()  {
        setSceneRect(0.0, 0.0, 0.0, 0.0);
        startpoint = new QPointF(0,0);
        endpoint = new QPointF(255, 0);
        prev = new QPointF(*startpoint);
        next = new QPointF(*endpoint);

    }

    TransferEditor::~TransferEditor(){}

    void TransferEditor::mousePressEvent(QGraphicsSceneMouseEvent* e) {
        this->update();
        press = new QPointF(e->scenePos());
        curr = new QPointF(e->scenePos());

        QPen* p = new QPen(Qt::darkCyan, 2.0, Qt::SolidLine, Qt::RoundCap);

        QGraphicsEllipseItem * ellips = new QGraphicsEllipseItem(e->scenePos().x() - 5, e->scenePos().y() - 5, 10, 10);

        ellips->setPen(*p);
        this->addItem(ellips);
        
        std::stringstream ss;
        ss << "Press: " << e->scenePos().x() << ", " << e->scenePos().y();
        LogInfo(ss.str());
    }

    //QGraphicsEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *e){
    //    LogInfo("Boink");
    //}

    void TransferEditor::mouseMoveEvent(QGraphicsSceneMouseEvent *e){
        QPointF* release = new QPointF(e->scenePos());

        int x1 = release->x();
        int y1 = release->y();
        int x2 = press->x();
        int y2 = press->y();

        QRect* rect = new QRect();

        if (x1 < x2){
            rect->setRight(x1);
            rect->setLeft(x2);
        } 
        else{
            rect->setRight(x2);
            rect->setLeft(x1);
        }

        if (y1 < y2){
            rect->setTop(y1);
            rect->setBottom(y2);
        } 
        else{        
            rect->setTop(y2);
            rect->setBottom(y1);
        }
        
        QPen* p = new QPen(QColor(60,60,60,50), 3.0, Qt::SolidLine, Qt::RoundCap);
        
        QGraphicsLineItem* line = new QGraphicsLineItem(0,0,450,300);
        
        line->setPen(*p);

        this->addItem(line);

        //this->clear();
        //this->addRect(*rect, *p);
        this->update();
        
        std::stringstream ss;
        ss << "Release: " << e->scenePos().x() << ", " << e->scenePos().y();
        LogInfo(ss.str());
        e->accept();
    }
}