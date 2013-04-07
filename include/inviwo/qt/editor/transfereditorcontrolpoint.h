#ifndef IVW_TRANSFEREDITORCONTROLPOINT_H
#define IVW_TRANSFEREDITORCONTROLPOINT_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QPainterPath>

#include <inviwo/core/ports/port.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>

namespace inviwo {

    class IVW_QTEDITOR_API TransferEditorControlPoint : public QGraphicsItem {

    public:
        TransferEditorControlPoint(QPointF startPoint);
        TransferEditorControlPoint(float x, float y);
        TransferEditorControlPoint();
        ~TransferEditorControlPoint();

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
        QPointF position();


    protected:
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    private:
        static const std::string logSource_;
        int id;
        int size;
    };

}// namespace
#endif // IVW_TRANSFEREDITORCONTROLPOINT_H