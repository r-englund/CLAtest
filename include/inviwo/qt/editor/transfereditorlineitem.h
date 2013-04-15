#ifndef IVW_TRANSFEREDITORLINEITEM_H
#define IVW_TRANSFEREDITORLINEITEM_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QPainterPath>

#include <inviwo/core/ports/port.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>
#include <inviwo/qt/editor/transfereditorcontrolpoint.h>

namespace inviwo {

    class IVW_QTEDITOR_API TransferEditorLineItem : public QGraphicsItem {

    public:
        TransferEditorLineItem(TransferEditorControlPoint*, TransferEditorControlPoint*);
        TransferEditorLineItem();
		~TransferEditorLineItem();
		void setStart(TransferEditorControlPoint* start_);
		void setFinish(TransferEditorControlPoint* finish_);

        //virtual QPainterPath shape() const;

        //override for qgraphicsitem_cast (refer qt documentation)
        enum { Type = UserType + ConnectionGraphicsType };
        int type() const  {return Type; }

        /**
        * Overloaded paint method from QGraphicsItem. Here the actual representation is drawn.
        */
        virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
        QRectF boundingRect() const;

        int getId();
        void setId(int id);
        QPointF getStart();
        QPointF getFinish();
        QPointF startPos;

    protected:
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);


    private:
        TransferEditorControlPoint* start;
        TransferEditorControlPoint* finish;
        
    };

}// namespace
#endif // IVW_TRANSFEREDITORLINEITEM_H