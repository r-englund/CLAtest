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

    TransferEditor::TransferEditor(PropertyWidgetQt *parent, TransferFunction* transferFunc, std::vector<TransferEditorControlPoint*>* points)
        :
		transferFunc_(transferFunc),
        parent_(parent),
        points_(points)
    {
        setSceneRect(0.0, 0.0, 0.0, 0.0);

        points_->push_back(new TransferEditorControlPoint(0, 0));
        points_->push_back(new TransferEditorControlPoint(255 , 0));
        calcTransferValues();
        (*points_)[0]->setId(0);
        (*points_)[1]->setId(1);
        (*points_)[0]->setZValue(1);
        (*points_)[1]->setZValue(1);
        addItem((*points_)[0]);
        addItem((*points_)[1]);

        for (int i = 0; i < (int)points_->size() - 1; i++)
        {
            lines_.push_back(new TransferEditorLineItem((*points_)[i], (*points_)[i+1]));
            lines_[i]->setZValue(0);
            addItem(lines_[i]);
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
        parent_->updateFromProperty();
    }

    void TransferEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent *e){
        QGraphicsScene::mouseReleaseEvent(e);
    }

    void TransferEditor::mouseMoveEvent(QGraphicsSceneMouseEvent *e){
        if ((*points_)[0]->pos().x() != 0)
            (*points_)[0]->setPos(QPoint(0, (*points_)[0]->pos().y()));
        
        sortLines();
        sortPoints();
        
        for (int i = 0; i < (int)points_->size() - 1; i++){
            lines_[i]->setStart((*points_)[i]);
            lines_[i]->setFinish((*points_)[i + 1]);
        }
        QGraphicsScene::mouseMoveEvent(e);
        calcTransferValues();
        parent_->updateFromProperty();
    }

    void TransferEditor::calcTransferValues(){
        //float* data = static_cast<float*>(transferFunc->getAlpha()->getData());
		
		Image* img = transferFunc_->getData();
		ImageRAMfloat32* imgRam = img->getEditableRepresentation<ImageRAMfloat32>();
		float* data = static_cast<float*>(imgRam->getData());

		glm::quat startQuat;
        glm::quat stopQuat;
        float factor;
        for (int i = 0; i < (int)points_->size() - 1; i++){
            QPointF start   = (*points_)[i]->position();
            QPointF stop    = (*points_)[i + 1]->position();

            for (int j = start.x(); j <=  stop.x(); j++){
                startQuat.x = start.y();
                stopQuat.x = stop.y();
                factor = (j - start.x())/(stop.x() - start.x());
                data[j] = glm::lerp(startQuat, stopQuat, factor).x/100.0f;
            }
        }
    }

    void TransferEditor::addPoint(QGraphicsSceneMouseEvent *e){
        points_->push_back(new TransferEditorControlPoint(e->scenePos()));
        lines_.push_back(new TransferEditorLineItem());
        (*points_)[points_->size() - 1]->setZValue(1);
        addItem(lines_[lines_.size() - 1]);
        addItem((*points_)[points_->size() - 1]);

        sortPoints();
        for (int i = 0; i < (int)points_->size() - 1; i++){
            lines_[i]->setStart((*points_)[i]);
            lines_[i]->setFinish((*points_)[i + 1]);
            lines_[i]->setZValue(0);
        }
        calcTransferValues();
    }

    void TransferEditor::removePoint(QGraphicsSceneMouseEvent *e){
        QPointF hit = itemAt(e->scenePos())->pos() * 2;
        QPointF curr;

        for (int i = 0; i < (int)points_->size() ; i++){
            curr = (*points_)[i]->position();
            if(curr.x() == hit.x() && curr.y() == hit.y()){
                if ((*points_)[i]->getId() != 0 && (*points_)[i]->getId() != 1)
                {
                    removeItem(lines_[i]);
                    lines_[i]->setVisible(false);

                    removeItem((*points_)[i]);

                    lines_.erase(lines_.begin() + i);
                    points_->erase(points_->begin() + i);
                    for (int j = 0; j < (int)points_->size() - 1; j++){
                        lines_[j]->setStart((*points_)[j]);
                        lines_[j]->setFinish((*points_)[j + 1]);
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
        std::sort((*points_).begin(), (*points_).end(), myPointCompare);
    }
    void TransferEditor::sortLines(){
        std::sort(lines_.begin(), lines_.end(), myLineCompare);
    }


};