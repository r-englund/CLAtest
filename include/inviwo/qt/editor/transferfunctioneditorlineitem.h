/** \TransferFunctionEditorLineItem class
 *         A line between control points in the TransferFunctionEditor
 *
 *         A QGraphicsItem that draws a line between the start_ and finish_ TransferFunctionEditorControlPoints
 *         
 */

#ifndef IVW_TRANSFERFUNCTIONEDITORLINEITEM_H
#define IVW_TRANSFERFUNCTIONEDITORLINEITEM_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QPainterPath>

#include <inviwo/core/ports/port.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>
#include <inviwo/qt/editor/transferfunctioneditorcontrolpoint.h>

namespace inviwo {

    class IVW_QTEDITOR_API TransferFunctionEditorLineItem : public QGraphicsItem {

    public:
        TransferFunctionEditorLineItem(TransferFunctionDataPoint*, TransferFunctionDataPoint*);
        TransferFunctionEditorLineItem();
		~TransferFunctionEditorLineItem();
		void setStart(TransferFunctionDataPoint* start);
		void setFinish(TransferFunctionDataPoint* finish);
        const QPointF* getStart();
        const QPointF* getFinish();


        //override for qgraphicsitem_cast (refer qt documentation)
        enum { Type = UserType + ConnectionGraphicsType };
        int type() const  {return Type; }

        virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
        QRectF boundingRect() const;

        int getId();
        void setId(int id);


    protected:
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);


    private:
        TransferFunctionDataPoint* start_;
        TransferFunctionDataPoint* finish_;
    };

}// namespace
#endif // IVW_TRANSFERFUNCTIONEDITORLINEITEM_H