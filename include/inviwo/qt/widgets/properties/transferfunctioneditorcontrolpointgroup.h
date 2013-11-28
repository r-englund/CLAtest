#ifndef IVW_TRANSFERFUNCTIONEDITORCONTROLPOINTGROUP_H
#define IVW_TRANSFERFUNCTIONEDITORCONTROLPOINTGROUP_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsView>
#include <QPainter>
#include <QPainterPath>

#include <inviwo/core/ports/port.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorcontrolpoint.h>

namespace inviwo {
    class IVW_QTEDITOR_API TransferFunctionEditorControlPointGroup : public QObject, public QGraphicsItem{
            Q_OBJECT
    public:
        TransferFunctionEditorControlPointGroup();
        ~TransferFunctionEditorControlPointGroup();

        //override for qgraphicsitem_cast (refer qt documentation)
        enum { Type = UserType + ConnectionGraphicsType };
        int type() const  {return Type; }

        /**Paint method
        * Overloaded paint method from QGraphicsItem. Here the actual representation is drawn.
        */
        void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
        QRectF boundingRect() const;


        void addPoint(TransferFunctionEditorControlPoint*);

		void notify();

    protected:
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	
    private:
        float size_; ///< size for drawing the points
        TransferFunctionDataPoint* datapoint_; ///<The TransferFunctionDataPoint the controlpoint gets all its data from
		float viewWidth_;
		float viewHeight_;
    };
}// namespace
#endif // IVW_TRANSFERFUNCTIONEDITORCONTROLPOINTGROUP_H