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
        Image* img = transferFunction_->getData();
        ImageRAMVec4float32 * imgRam = img->getEditableRepresentation<ImageRAMVec4float32>();
        data_ = static_cast<vec4*>(imgRam->getData());
    }

    TransferFunctionEditor::~TransferFunctionEditor(){}

    void TransferFunctionEditor::mousePressEvent(QGraphicsSceneMouseEvent *e)
    {
        if (e->button() == Qt::LeftButton){
            if (itemAt(e->scenePos()) == NULL){
                addPoint(e);
            }
            else{
                QGraphicsScene::mousePressEvent(e); // this forwards the event to the item
            }
        }
        if (e->button() == Qt::RightButton){
            if (itemAt(e->scenePos()) == NULL){}
            else if (itemAt(e->scenePos())->type() == TransferFunctionEditorControlPoint::Type) {
                removePoint(e);
            }
        }
        parent_->updateFromProperty();
    }

    void TransferFunctionEditor::mouseMoveEvent(QGraphicsSceneMouseEvent *e){
        QGraphicsScene::mouseMoveEvent(e);
        transferFunction_->sortDataPoints();
        transferFunction_->calcTransferValues();
        parent_->updateFromProperty();
        for (int i = 0; i < transferFunction_->getSize() - 1; i++){
            lines_[i]->setStart(transferFunction_->getPoint(i));
            lines_[i]->setFinish(transferFunction_->getPoint(i+1));
        }
        parent_->updateFromProperty();
        this->update();
    }

    void TransferFunctionEditor::addPoint(QGraphicsSceneMouseEvent *e){
        vec2* pos = new vec2(e->scenePos().x(), e->scenePos().y());
        vec4* rgba = new vec4(e->scenePos().y()/100.0f);
        TransferFunctionDataPoint* newPoint = new TransferFunctionDataPoint(pos, rgba);
        transferFunction_->addPoint(newPoint);
        points_->push_back(new TransferFunctionEditorControlPoint(newPoint));
        addItem(points_->back());
        if (transferFunction_->getSize() > 1){
            lines_.push_back(new TransferFunctionEditorLineItem(
                transferFunction_->getPoint(transferFunction_->getSize()-2), 
                transferFunction_->getPoint(transferFunction_->getSize()-1)));
            addItem(lines_.back());
        }
        for (int i = 0; i < transferFunction_->getSize() - 1; i++){
            lines_[i]->setStart(transferFunction_->getPoint(i));
            lines_[i]->setFinish(transferFunction_->getPoint(i+1));
        }
        parent_->updateFromProperty();
        this->update();
    }

    void TransferFunctionEditor::removePoint(QGraphicsSceneMouseEvent *e){
        TransferFunctionEditorControlPoint* target = (TransferFunctionEditorControlPoint*)itemAt(e->scenePos());
        std::vector<TransferFunctionEditorControlPoint*>::iterator iter = points_->begin();

        lines_.back()->setVisible(false);
        lines_.pop_back();

        for (iter = points_->begin(); iter != points_->end(); iter++){
            if (target->pos() == (*iter)->pos()){
                transferFunction_->removePoint((*iter)->getPoint());
                (*iter)->setVisible(false);
                points_->erase(iter);
                break;
            }
        }

        transferFunction_->sortDataPoints();
        transferFunction_->calcTransferValues();
        parent_->updateFromProperty();
        for (int i = 0; i < transferFunction_->getSize() - 1; i++){
            lines_[i]->setStart(transferFunction_->getPoint(i));
            lines_[i]->setFinish(transferFunction_->getPoint(i+1));
        }
        this->update();
    }
};