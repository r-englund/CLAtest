#ifndef IVW_TRANSFEREDITOR_H
#define IVW_TRANSFEREDITOR_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointF>

#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/ports/port.h>

namespace inviwo {

	//class CanvasQt;

    class TransferEditor : public QGraphicsScene {

    public :
        TransferEditor();
        ~TransferEditor();

    protected :
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *MouseEvent);

    private :
        static const std::string logSource_; ///< Source string to be displayed for log messages.
        QPointF* startpoint;
        QPointF* endpoint;
        QPointF* prev;
        QPointF* next;
        QPointF* curr;
        QPointF* press;
    };

} // namespace

#endif // IVW_TRANSFEREDITOR_H