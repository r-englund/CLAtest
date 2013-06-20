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
        /** 
         * \brief TransferFunctionEditorLineItem Constructor
         *
         * Constructor for TransferFunctionEditorLineItem
         * 
         * @param TransferFunctionDataPoint * Pointer to the point where the line starts
         * @param TransferFunctionDataPoint * Pointer to the point where the line ends
         */
        TransferFunctionEditorLineItem(TransferFunctionEditorControlPoint*, TransferFunctionEditorControlPoint*);
        /** 
         * \brief TransferFunctionEditorLineItem Constructor
         *
         * TransferFunctionEditorLineItem default constructor
         */
        TransferFunctionEditorLineItem();
		~TransferFunctionEditorLineItem();
		/** 
		 * \brief Sets the lines starting point
		 *
		 * Changes the point where the line starts
		 * 
		 * @param TransferFunctionDataPoint * start the new start-point
		 */
		void setStart(TransferFunctionEditorControlPoint* start);

		/** 
		 * \brief Sets the lines ending point
		 *
		 * Changes the point where the line ends
		 * 
		 * @param TransferFunctionDataPoint * finish the new end-point
		 */
		void setFinish(TransferFunctionEditorControlPoint* finish);
        /** 
         * \brief returns the start-position
         *
         * Returns the position of the start-point as a QPointF
         * 
         * @return const QPointF position of start-point
         */
        //const QPointF getStart();
        //const QPointF getFinish();


        //override for qgraphicsitem_cast (refer qt documentation)
        enum { Type = UserType + ConnectionGraphicsType + 1};
        int type() const  {return Type; }

        virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
        QRectF boundingRect() const;

        int getId();
        void setId(int id);


    protected:
        void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent);
        void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent);

    private:
        TransferFunctionEditorControlPoint* start_;
        TransferFunctionEditorControlPoint* finish_;
		QPointF mouseDownPos_;
    };

}// namespace
#endif // IVW_TRANSFERFUNCTIONEDITORLINEITEM_H