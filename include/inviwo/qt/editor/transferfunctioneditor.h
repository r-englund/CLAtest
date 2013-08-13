/** \Inherited from QGraphicsScene for editing a transfer function (TransferFunction)
*
*  Class for graphical configuration of a transfer function
*  
*/

#ifndef IVW_TRANSFERFUNCTIONEDITOR_H
#define IVW_TRANSFERFUNCTIONEDITOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/io/serialization/ivwserializer.h>
#include <inviwo/core/io/serialization/ivwdeserializer.h>
#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/util/inviwofactorybase.h>

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/qt/editor/transferfunctioneditorcontrolpoint.h>
#include <inviwo/qt/editor/transferfunctioneditorlineitem.h>

#include <inviwo/qt/editor/propertylistwidget.h>

#include <QBrush>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QLineF>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <vector>

#include <inviwo/core/util/observer.h>

//#include <vld.h>

namespace inviwo {
    class TransferFunctionEditor :public QGraphicsScene, public VoidObservable{

        Q_OBJECT

    public :
        /** \TransferFunctionEditor constructor
        *         
        *  Default TransferFunctionEditor constructor
        */
        TransferFunctionEditor();

        /** \TransferFunctionEditor constructor
        *         
        *  Main constructor, creates edge control points and corresponding line
        */
        TransferFunctionEditor(TransferFunction* transferFunction_);
        ~TransferFunctionEditor();
    
    signals:
        void doubleClick();


	protected :
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);
		void mousePressEvent(QGraphicsSceneMouseEvent *e);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
		void keyPressEvent(QKeyEvent *e);

        //void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

        /** \Add new control point
        *      Adds a new control point at the event position
        *
        *      Adds a new control point the the points_ array, adds a new line item to the lines_ array,
        *      sorts the points_ array and updates the line items to go to and from the correct points.
        *      Runs CalcTransferValues to update the TransferFunction datamember Image
		*/
		void addPoint(QGraphicsSceneMouseEvent *e);
		void addPoint(vec2* pos);
		void removePoint(QGraphicsSceneMouseEvent *e);
		std::vector<TransferFunctionEditorControlPoint*>::iterator removePoint(TransferFunctionEditorControlPoint* p);
		void sortLines();
		void sortControlPoints();
		void setControlPointNeighbours();

    private :
        std::vector<TransferFunctionEditorControlPoint*> points_; ///< Control points in the transfer function graph
        std::vector<TransferFunctionEditorLineItem*> lines_; ///< Vector for the lines between the controlpoints
        TransferFunction* transferFunction_; ///< Pointer to widget's member variable
		QPointF mouseDownPos_; ///< Stores the mouseDown position to distinguish between mouse-click and mouse-drag
		TransferFunctionEditorLineItem* leftEdgeLine_;
		TransferFunctionEditorLineItem*	rightEdgeLine_;
    };

} // namespace

#endif // IVW_TRANSFERFUNCTIONEDITOR_H