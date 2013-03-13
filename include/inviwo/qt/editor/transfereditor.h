#ifndef IVW_TRANSFEREDITOR_H
#define IVW_TRANSFEREDITOR_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointF>

#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/ports/port.h>

#include <vector>
#include <inviwo/qt/editor/transfereditorgraphicsitem.h>
#include <inviwo/qt/editor/transfereditorlineitem.h>

namespace inviwo {
    class TransferEditor : public QGraphicsScene {

    public :
        TransferEditor();
        ~TransferEditor();
        void sortPoints();
        bool comparer(TransferEditorGraphicsItem* a, TransferEditorGraphicsItem* b);
        bool comparer2(TransferEditorGraphicsItem a, TransferEditorGraphicsItem b);

    protected :
        void mousePressEvent(QGraphicsSceneMouseEvent *e);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
        void addPoint(QGraphicsSceneMouseEvent *e);

    private :
        static const std::string logSource_; ///< Source string to be displayed for log messages.
        QPointF* startpoint;
        QPointF* endpoint;
        QPointF* press;
        vector<TransferEditorGraphicsItem*> points;
        vector<TransferEditorLineItem*> lines;
    };

} // namespace

#endif // IVW_TRANSFEREDITOR_H