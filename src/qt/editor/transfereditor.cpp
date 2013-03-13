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

#include <stdlib.h>

namespace inviwo {

    const std::string TransferEditor::logSource_ = "TransferEditor";

    TransferEditor::TransferEditor()  {
        setSceneRect(0.0, 0.0, 0.0, 0.0);
        startpoint = new QPointF(0,0);
        endpoint = new QPointF(255, 0);

        for (int i = 0; i < 5; i++)
        {
            points.push_back(new TransferEditorGraphicsItem(32 * i, 25));
            points[i]->setId(i);
            addItem(points[i]);
        }

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

        if (itemAt(e->scenePos()) == NULL){
            addPoint(e);
        }
        else{
            QGraphicsScene::mousePressEvent(e); // this forwards the event to the item
        }
    }

	void TransferEditor::addPoint(QGraphicsSceneMouseEvent *e){
		points.push_back(new TransferEditorGraphicsItem(e->scenePos()/2));
		lines.push_back(new TransferEditorLineItem());
		addItem(lines[lines.size() - 1]);
		addItem(points[points.size() - 1]);
		sortPoints();
		for (int i = 0; i < (int)points.size() - 1; i++){
			lines[i]->setStart(points[i]);
			lines[i]->setFinish(points[i + 1]);
		}
		//this->update();
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
        sortPoints();
		for (int i = 0; i < (int)points.size() - 1; i++){
			lines[i]->setStart(points[i]);
			lines[i]->setFinish(points[i + 1]);
		}

		//for (int i = 0; i < (int)points.size(); i++)
		//{
		//	LogInfo(points[i]->pos().x());
		//}

        QGraphicsScene::mouseMoveEvent(e);
    }

  
	bool myCompare (const TransferEditorGraphicsItem * a, const TransferEditorGraphicsItem* b){
		return a->pos().x() < b->pos().x();
		//return true;
	}

	void TransferEditor::sortPoints(){
		std::sort(points.begin(), points.end(), myCompare);
    }
};