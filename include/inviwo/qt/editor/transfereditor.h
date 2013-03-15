#ifndef IVW_TRANSFEREDITOR_H
#define IVW_TRANSFEREDITOR_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointF>

#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/ports/port.h>

#include <inviwo/core/ports/imageport.h>
#include <vector>
#include <valarray>
#include <inviwo/qt/editor/transfereditorgraphicsitem.h>
#include <inviwo/qt/editor/transfereditorlineitem.h>

namespace inviwo {
    class TransferEditor : public QGraphicsScene {

    public :
        TransferEditor();
        ~TransferEditor();
        vector<int> getTransferValues();
        void sortPoints();
        void sortLines();

    protected :
        void mousePressEvent(QGraphicsSceneMouseEvent *e);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

        void addPoint(QGraphicsSceneMouseEvent *e);
        void removePoint(QGraphicsSceneMouseEvent *e);
        void calcTransferArray();
        float linearInterpolation(QPointF, QPointF, int);

    private :
        static const std::string logSource_;
        vector<TransferEditorGraphicsItem*> points;
        vector<TransferEditorLineItem*> lines;
        vector<int> transferValues;        
    };

} // namespace

#endif // IVW_TRANSFEREDITOR_H