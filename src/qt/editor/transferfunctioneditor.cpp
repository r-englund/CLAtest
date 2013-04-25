//#include <inviwo/core/inviwoapplication.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/io/serialization/ivwserializer.h>
#include <inviwo/core/io/serialization/ivwdeserializer.h>
#include <inviwo/qt/editor/processorlistwidget.h>
#include <inviwo/qt/editor/propertylistwidget.h>
#include <inviwo/qt/widgets/processors/processorwidgetfactoryqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
#include <inviwo/core/metadata/positionmetadata.h>
#include <inviwo/core/util/inviwofactorybase.h>
#include <inviwo/qt/editor/transferfunctioneditor.h>

#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>
#include <inviwo/core/properties/vectorproperties.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/ordinalproperty.h>

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

    TransferFunctionEditor::TransferFunctionEditor(PropertyWidgetQt *parent, TransferFunction* transferFunc, std::vector<TransferFunctionEditorControlPoint*>* points)
        :
        transferFunction_(transferFunc),
        parent_(parent),
        points_(points)
    {
        //colorpicker_ = new IntVec4Property();
        //colorpicker_.setSemantics(PropertySemantics::Color);
        Image* img = transferFunction_->getData();
        ImageRAMVec4float32 * imgRam = img->getEditableRepresentation<ImageRAMVec4float32>();
        data_ = static_cast<vec4*>(imgRam->getData());

        points_->push_back(new TransferFunctionEditorControlPoint(0.0f, 0.0f, data_));
        points_->push_back(new TransferFunctionEditorControlPoint(255.0f , 100.0f, data_));

        data_[0] = vec4(0.0f, 0.0f, 0.0f, 0.0f);
        data_[255] = vec4(0.0f, 1.0f, 1.0f, 1.0f);

        (*points_)[0]->setId(0);
        (*points_)[1]->setId(1);

        calcTransferValues();
        (*points_)[0]->setZValue(1);
        (*points_)[1]->setZValue(1);
        addItem((*points_)[0]);
        addItem((*points_)[1]);

        lines_.push_back(new TransferFunctionEditorLineItem((*points_)[0], (*points_)[1]));
        lines_[0]->setZValue(0);
        addItem(lines_[0]);

    }

    TransferFunctionEditor::~TransferFunctionEditor(){}

    void TransferFunctionEditor::mousePressEvent(QGraphicsSceneMouseEvent *e)
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
            else if(itemAt(e->scenePos())->type() == TransferFunctionEditorControlPoint::Type) {
                removePoint(e);
            }
        }
        parent_->updateFromProperty();
    }

    void TransferFunctionEditor::mouseMoveEvent(QGraphicsSceneMouseEvent *e){
        sortLines();
        sortPoints();
        for (int i = 0; i < (int)points_->size() - 1; i++){
            lines_[i]->setStart((*points_)[i]);
            lines_[i]->setFinish((*points_)[i + 1]);
        }
        QGraphicsScene::mouseMoveEvent(e);
        calcTransferValues();
        parent_->updateFromProperty();
        //parent_->setProperty();
    }

    void TransferFunctionEditor::addPoint(QGraphicsSceneMouseEvent *e){
        points_->push_back(new TransferFunctionEditorControlPoint(new QPointF(e->scenePos()), data_));
        lines_.push_back(new TransferFunctionEditorLineItem());

        data_[(int)e->scenePos().x()] = vec4(e->scenePos().y()/100.0f);

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

    void TransferFunctionEditor::removePoint(QGraphicsSceneMouseEvent *e){
        TransferFunctionEditorControlPoint* target = (TransferFunctionEditorControlPoint*)itemAt(e->scenePos());
        const QPointF* hit = target->getPosition();
        const QPointF* curr;

        if (target->getId() != 0 && target->getId() != 1)
        {
            for (int i = 0; i < (int)points_->size() ; i++){
                curr = (*points_)[i]->getPosition();
                if(curr->x() == hit->x() && curr->y() == hit->y()){
                    removeItem(lines_[i]);
                    lines_[i]->setVisible(false);
                    removeItem((*points_)[i]);
                    target->setVisible(false);

                    lines_.erase(lines_.begin() + i);
                    points_->erase(points_->begin() + i);
                    for (int j = 0; j < (int)points_->size() - 1; j++){
                        lines_[j]->setStart((*points_)[j]);
                        lines_[j]->setFinish((*points_)[j + 1]);
                    }
                }
            }
        }
        calcTransferValues();
        this->update();
    }

    void TransferFunctionEditor::calcTransferValues(){
        //transferFunction_->calcTransferValues();

        vec4* newValues;
        std::stringstream ss;
        glm::quat startQuat;
        glm::quat stopQuat;
        float factor;

        //Loops through all point to point intervals
        for (int i = 0; i < (int)points_->size() - 1; i++){
            const QPointF* start = (*points_)[i]->getPosition();
            const QPointF* stop = (*points_)[i + 1]->getPosition();

            vec4 startValues = data_[(int)(*points_)[i]->getPosition()->x()];
            vec4 stopValues = data_[(int)(*points_)[i + 1]->getPosition()->x()];
            
            startValues.a = (*points_)[i]->getPosition()->y()/100.0f;
            stopValues.a = (*points_)[i + 1]->getPosition()->y()/100.0f;

            //Interpolates the function values for all intermediate positions
            for (int j = start->x(); j <=  stop->x(); j++){
                factor = (j - start->x())/(stop->x() - start->x());

                startQuat.x = startValues.a;
                stopQuat.x = stopValues.a;
                float newA = glm::lerp(startQuat, stopQuat, factor).x;
                startQuat.x = startValues.r;
                stopQuat.x = stopValues.r;
                float newR = glm::lerp(startQuat, stopQuat, factor).x; 
                startQuat.x = startValues.b;
                stopQuat.x = stopValues.b;
                float newB = glm::lerp(startQuat, stopQuat, factor).x;
                startQuat.x = startValues.g;
                stopQuat.x = stopValues.g;
                float newG = glm::lerp(startQuat, stopQuat, factor).x;
                newValues = new vec4(newR, newB, newG, newA);
                data_[j] = *newValues;
            }
        }
    }

    /** \Custom comparator
    *         
    *  A custom comparer for sorting points by position x-component
    */
    bool myPointCompare (TransferFunctionEditorControlPoint * a, TransferFunctionEditorControlPoint* b){
        return a->getPosition()->x() < b->getPosition()->x();
    }

    /** \Custom comparator
    *         
    *  A custom comparer for sorting lines by position x-component
    */
    bool myLineCompare (TransferFunctionEditorLineItem * a, TransferFunctionEditorLineItem * b){
        return a->getStart()->x() < b->getStart()->x();
    }

    void TransferFunctionEditor::sortPoints(){
        std::sort((*points_).begin(), (*points_).end(), myPointCompare);
    }

    void TransferFunctionEditor::sortLines(){
        std::sort(lines_.begin(), lines_.end(), myLineCompare);
    }
};