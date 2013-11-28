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
#include <inviwo/core/util/inviwofactorybase.h>

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorcontrolpoint.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorcontrolpointgroup.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorlineitem.h>

#include <inviwo/qt/widgets/propertylistwidget.h>

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
        TransferFunctionEditor(TransferFunction* transferFunction, QGraphicsView* view);
        ~TransferFunctionEditor();
		QGraphicsView* getView();
		void repositionPoints();
        const float getZoomRangeXMin();
        const float getZoomRangeXMax();
        void setZoomRangeXMin(float min);
        void setZoomRangeXMax(float max);

        const float getZoomRangeYMin();
        const float getZoomRangeYMax();
        void setZoomRangeYMin(float min);
        void setZoomRangeYMax(float max);
    
    signals:
        void doubleClick();

	protected :
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);
		void mousePressEvent(QGraphicsSceneMouseEvent *e);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
		void keyPressEvent(QKeyEvent *e);
		void ResizeEvent(QResizeEvent * e);

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
        float zoomRangeXMin_;
        float zoomRangeXMax_;
        float zoomRangeYMin_;
        float zoomRangeYMax_;
		QGraphicsView* view_;
		QPointF mouseDownPos_; ///< Stores the mouseDown position to distinguish between mouse-click and mouse-drag
		TransferFunction* transferFunction_; ///< Pointer to widget's member variable
		TransferFunctionEditorLineItem* leftEdgeLine_;
		TransferFunctionEditorLineItem*	rightEdgeLine_;
		std::vector<TransferFunctionEditorLineItem*> lines_; ///< Vector for the lines between the controlpoints
		std::vector<TransferFunctionEditorControlPoint*> controlPoints_; ///< Control points in the transfer function graph
		std::vector<TransferFunctionEditorControlPointGroup*> controlPointGroups_; ///< Control points in the transfer function graph
    };
} // namespace

#endif // IVW_TRANSFERFUNCTIONEDITOR_H