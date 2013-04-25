/** \Inherited from QGraphicsScene for editing a transfer function (TransferFunction)
*
*  Class for graphical configuration of a transfer function
*  
*/

#ifndef IVW_TRANSFERFUNCTIONEDITOR_H
#define IVW_TRANSFERFUNCTIONEDITOR_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointF>

#include<vector>

#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/ports/port.h>

#include <inviwo/core/ports/imageport.h>
#include <inviwo/qt/editor/transferfunctioneditorcontrolpoint.h>
#include <inviwo/qt/editor/transferfunctioneditorlineitem.h>

namespace inviwo {
    class TransferFunctionEditor : public QGraphicsScene {

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
        TransferFunctionEditor(PropertyWidgetQt *parent_, TransferFunction* transferFunction_, std::vector<TransferFunctionEditorControlPoint*>* points_);
        ~TransferFunctionEditor();

        /** \Custom sort
        *         
        *  A sorter that uses myPointCompare to sort points by position x-component
        */
        void sortPoints();

        /** \Custom sort
        *         
        *  A sorter that uses myLineCompare to sort lines by position x-component
        */
        void sortLines();

    protected :
        void mousePressEvent(QGraphicsSceneMouseEvent *e);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
        //void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

        /** \Add new control point
        *      Adds a new control point at the event position
        *
        *      Adds a new control point the the points_ array, adds a new line item to the lines_ array,
        *      sorts the points_ array and updates the line items to go to and from the correct points.
        *      Runs CalcTransferValues to update the TransferFunction datamember Image
        */
        void addPoint(QGraphicsSceneMouseEvent *e);
        void removePoint(QGraphicsSceneMouseEvent *e);

        /** \Calculates the transfer function values
        *   Extracts the Image data to a float array,
        *   Loops through the controlpoints and sets the array to the calculated values
        */
        void calcTransferValues();

    private :
        std::vector<TransferFunctionEditorControlPoint*>* points_; ///< Control points in the transfer function graph
        std::vector<TransferFunctionEditorLineItem*> lines_; ///< Vector for the lines between the controlpoints
        TransferFunction* transferFunction_; ///< Pointer to widget's member variable
        PropertyWidgetQt *parent_; ///< Pointer to widget so it can be updated from the editor
        vec4* data_;
        //IntVec4Property colorpicker_;


    };

} // namespace

#endif // IVW_TRANSFERFUNCTIONEDITOR_H