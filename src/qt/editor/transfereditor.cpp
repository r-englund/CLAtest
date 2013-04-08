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

#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>

#include <QApplication>
#include <QBrush>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QLineF>
#include <QMenu>
#include <QPainter>
#include <QPen>
#include <stdlib.h>

namespace inviwo {

    const std::string TransferEditor::logSource_ = "TransferEditor";

    TransferEditor::TransferEditor(PropertyWidgetQt *parent_, TransferFunction* transferFunc_, std::vector<TransferEditorControlPoint*>* points_)
        :transferFunc(transferFunc_),
        parent(parent_),
        points(points_)
    {
        setSceneRect(0.0, 0.0, 0.0, 0.0);

        points->push_back(new TransferEditorControlPoint(0, 0));
        points->push_back(new TransferEditorControlPoint(255 , 0));
        calcTransferValues();
        (*points)[0]->setId(0);
        (*points)[1]->setId(1);
        (*points)[0]->setZValue(1);
        (*points)[1]->setZValue(1);
        addItem((*points)[0]);
        addItem((*points)[1]);

        for (int i = 0; i < (int)points->size() - 1; i++)
        {
            lines.push_back(new TransferEditorLineItem((*points)[i], (*points)[i+1]));
            lines[i]->setZValue(0);
            addItem(lines[i]);
        }
    }

    TransferEditor::~TransferEditor(){}

    void TransferEditor::mousePressEvent(QGraphicsSceneMouseEvent *e)
    {
        std::stringstream ss;

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
            else if(itemAt(e->scenePos())->type() == TransferEditorControlPoint::Type) {
                removePoint(e);
            }
        }
        parent->updateFromProperty();
    }

    void TransferEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
        QGraphicsScene::mouseReleaseEvent(e);
    }

    void TransferEditor::mouseMoveEvent(QGraphicsSceneMouseEvent *e){
        if ((*points)[0]->pos().x() != 0)
            (*points)[0]->setPos(QPoint(0, (*points)[0]->pos().y()));
        
        sortLines();
        sortPoints();
        
        for (int i = 0; i < (int)points->size() - 1; i++){
            lines[i]->setStart((*points)[i]);
            lines[i]->setFinish((*points)[i + 1]);
        }
        QGraphicsScene::mouseMoveEvent(e);
        calcTransferValues();
        parent->updateFromProperty();
    }

    void TransferEditor::calcTransferValues(){
        float* data = static_cast<float*>(transferFunc->getAlpha()->getData());
        glm::quat startQuat;
        glm::quat stopQuat;
        float factor;
        for (int i = 0; i < (int)points->size() - 1; i++){
            QPointF start   = (*points)[i]->position();
            QPointF stop    = (*points)[i + 1]->position();

            for (int j = start.x(); j <=  stop.x(); j++){
                startQuat.x = start.y();
                stopQuat.x = stop.y();
                factor = (j - start.x())/(stop.x() - start.x());
                data[j] = glm::lerp(startQuat, stopQuat, factor).x/100.0f;
            }
        }
    }

    void TransferEditor::addPoint(QGraphicsSceneMouseEvent *e){
        points->push_back(new TransferEditorControlPoint(e->scenePos()));
        lines.push_back(new TransferEditorLineItem());
        (*points)[points->size() - 1]->setZValue(1);
        addItem(lines[lines.size() - 1]);
        addItem((*points)[points->size() - 1]);

        sortPoints();
        for (int i = 0; i < (int)points->size() - 1; i++){
            lines[i]->setStart((*points)[i]);
            lines[i]->setFinish((*points)[i + 1]);
            lines[i]->setZValue(0);
        }
        calcTransferValues();
    }

    void TransferEditor::removePoint(QGraphicsSceneMouseEvent *e){
        QPointF hit = itemAt(e->scenePos())->pos() * 2;
        QPointF curr;

        for (int i = 0; i < (int)points->size() ; i++){
            curr = (*points)[i]->position();
            if(curr.x() == hit.x() && curr.y() == hit.y()){
                if ((*points)[i]->getId() != 0 && (*points)[i]->getId() != 1)
                {
                    removeItem(lines[i]);
                    lines[i]->setVisible(false);

                    removeItem((*points)[i]);

                    lines.erase(lines.begin() + i);
                    points->erase(points->begin() + i);
                    for (int j = 0; j < (int)points->size() - 1; j++){
                        lines[j]->setStart((*points)[j]);
                        lines[j]->setFinish((*points)[j + 1]);
                    }
                    update();
                }
            }
        }
    }

    bool myPointCompare (const TransferEditorControlPoint * a, const TransferEditorControlPoint* b){
        return a->pos().x() < b->pos().x();
    }

    bool myLineCompare (const TransferEditorLineItem * a, const TransferEditorLineItem * b){
        return a->startPos.x() < b->startPos.x();
    }
    void TransferEditor::sortPoints(){
        std::sort((*points).begin(), (*points).end(), myPointCompare);
    }
    void TransferEditor::sortLines(){
        std::sort(lines.begin(), lines.end(), myLineCompare);
    }


};