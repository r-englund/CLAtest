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
#include <QGraphicsView>
#include <stdlib.h>

namespace inviwo {

    const std::string TransferEditor::logSource_ = "TransferEditor";

    TransferEditor::TransferEditor(){
        transferValues.resize(256);
        fill(transferValues.begin(), transferValues.end(), 0);
        setSceneRect(0.0, 0.0, 0.0, 0.0);

        points.push_back(new TransferEditorGraphicsItem(0, 0));
        points.push_back(new TransferEditorGraphicsItem(255 , 0));
        points[0]->setId(0);
        points[1]->setId(1);
        addItem(points[0]);
        addItem(points[1]);

        for (int i = 0; i < (int)points.size() - 1; i++)
        {
            lines.push_back(new TransferEditorLineItem(points[i], points[i+1]));
            addItem(lines[i]);
        }
    }

    TransferEditor::~TransferEditor(){}

    void TransferEditor::mousePressEvent(QGraphicsSceneMouseEvent *e)
    {
        update();
        std::stringstream ss;

        //65538
        //if (itemAt(e->scenePos()) != NULL)
        //    LogInfo(itemAt(e->scenePos())->type());

        if (e->button() == Qt::LeftButton){
            if (itemAt(e->scenePos()) == NULL){
                addPoint(e);
            }
            else{
                QGraphicsScene::mousePressEvent(e); // this forwards the event to the item
            }
        }
        if(e->button() == Qt::RightButton){
            if (itemAt(e->scenePos()) == NULL){}
            else if(itemAt(e->scenePos())->type() == 65538) {
                removePoint(e);
            }
        }
    }

    void TransferEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
        QGraphicsScene::mouseReleaseEvent(e);
    }


    void TransferEditor::mouseMoveEvent(QGraphicsSceneMouseEvent *e){
        if (points[0]->pos().x() != 0)
        {
            points[0]->setPos(QPoint(0, points[0]->pos().y()));
        }
        update();
        sortLines();
        sortPoints();
        for (int i = 0; i < (int)points.size() - 1; i++){
            lines[i]->setStart(points[i]);
            lines[i]->setFinish(points[i + 1]);
        }
        QGraphicsScene::mouseMoveEvent(e);
        calcTransferArray();
    }


    bool myPointCompare (const TransferEditorGraphicsItem * a, const TransferEditorGraphicsItem* b){
        return a->pos().x() < b->pos().x();
    }

    bool myLineCompare (const TransferEditorLineItem * a, const TransferEditorLineItem * b){
        return a->startPos.x() < b->startPos.x();
    }

    void TransferEditor::calcTransferArray(){
        stringstream ss;
        ss << endl;
        for (int i = 0; i < (int)points.size() - 1; i++){
            QPointF start   = points[i]     ->position();
            QPointF stop    = points[i + 1] ->position();

            for (int j = start.x(); j <=  stop.x(); j++){
                transferValues[j] = linearInterpolation(start, stop, j);
            }
        }
    }

    void TransferEditor::addPoint(QGraphicsSceneMouseEvent *e){
        points.push_back(new TransferEditorGraphicsItem(e->scenePos()));
        lines.push_back(new TransferEditorLineItem());
        addItem(lines[lines.size() - 1]);
        addItem(points[points.size() - 1]);
        sortPoints();
        for (int i = 0; i < (int)points.size() - 1; i++){
            lines[i]->setStart(points[i]);
            lines[i]->setFinish(points[i + 1]);
            LogInfo(lines[i]->getStart().x());
        }
        calcTransferArray();
    }

    void TransferEditor::removePoint(QGraphicsSceneMouseEvent *e){
        stringstream ss;
        QPointF hit = itemAt(e->scenePos())->pos() * 2;
        QPointF curr;


        for (int i = 0; i < (int)points.size() ; i++){
            curr = points[i]->position();
            if(curr.x() == hit.x() && curr.y() == hit.y()){
                if (points[i]->getId() != 0 && points[i]->getId() != 1)
                {
                    removeItem(lines[i]);
                    lines[i]->setVisible(false);

                    removeItem(points[i]);
                    lines.erase(lines.begin() + i);
                    points.erase(points.begin() + i);
                    for (int j = 0; j < (int)points.size() - 1; j++){
                        lines[j]->setStart(points[j]);
                        lines[j]->setFinish(points[j + 1]);
                    }
                    update();
                }
            }
        }
    }

    float TransferEditor::linearInterpolation(QPointF p0, QPointF p1, int posx){
        float factor = (posx - p0.x())/(p1.x() - p0.x());
        return (1 - factor) * p0.y() + factor * p1.y();
    }

    vector<int> TransferEditor::getTransferValues(){
        return transferValues;

    }

    void TransferEditor::sortPoints(){
        std::sort(points.begin(), points.end(), myPointCompare);
    }
    void TransferEditor::sortLines(){
        std::sort(lines.begin(), lines.end(), myLineCompare);
    }

};