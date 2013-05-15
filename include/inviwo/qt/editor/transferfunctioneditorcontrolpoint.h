/** \TransferFunctionEditorControlPoint class
 *         A point in the TransferFunctionEditor
 *
 *         Overloaded QGraphicsItem used in TransferFunctionEditor. Position of control points are used to
 *         calulate the TransferFunction Image values, currently has one parameter
 */
#ifndef IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H
#define IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QPainterPath>

#include <inviwo/core/ports/port.h>
#include <inviwo/qt/editor/processorgraphicsitem.h>

namespace inviwo {

    class IVW_QTEDITOR_API TransferFunctionEditorControlPoint : public QGraphicsItem {

    public:
        /** \TransferFunctionEditorControlPoint constructor  
        *      Creates a TransferFunctionEditorControlPoint at a QPointF
        */        
        TransferFunctionEditorControlPoint(TransferFunctionDataPoint* datapoint);
        
        /** \TransferFunctionEditorControlPoint constructor  
        *      Default constructor
        */
        TransferFunctionEditorControlPoint();

        /** \TransferFunctionEditorControlPoint destructor  
        */
        ~TransferFunctionEditorControlPoint();

        //override for qgraphicsitem_cast (refer qt documentation)
        enum { Type = UserType + ConnectionGraphicsType };
        int type() const  {return Type; }

        /**Paint method
        * Overloaded paint method from QGraphicsItem. Here the actual representation is drawn.
        */
        virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
        QRectF boundingRect() const;

        TransferFunctionDataPoint* getPoint();
        /**
        * get/set functions for id and position
        */
        const int getId();
        void setId(int id);
        //void setPosition(QPointF *);
        //const QPointF* getPosition();

    protected:
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

    private:
        int id_; ///< id to identify edge points
        int size_; ///< size for drawing the points
        //QPointF* pos_;///< position
        //vec4* data_;///< Pointer to where the controlpoint gets its color value
        TransferFunctionDataPoint* datapoint_;
    };

}// namespace
#endif // IVW_TRANSFERFUNCTIONEDITORCONTROLPOINT_H