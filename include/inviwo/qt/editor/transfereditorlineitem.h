#ifndef IVW_TRANSFEREDITORLINEITEM_H
#define IVW_TRANSFEREDITORLINEITEM_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QPainterPath>

#include <inviwo/core/ports/port.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>
#include <inviwo/qt/editor/transfereditorgraphicsitem.h>

namespace inviwo {

    class IVW_QTEDITOR_API TransferEditorLineItem : public QGraphicsItem {

    public:
        TransferEditorLineItem(TransferEditorGraphicsItem*, TransferEditorGraphicsItem*);
        TransferEditorLineItem();
		~TransferEditorLineItem();
		void setStart(TransferEditorGraphicsItem* start_);
		void setFinish(TransferEditorGraphicsItem* finish_);

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
        static const std::string logSource_;
        TransferEditorGraphicsItem* start;
        TransferEditorGraphicsItem* finish;
        
    };

}// namespace
#endif // IVW_TRANSFEREDITORLINEITEM_H