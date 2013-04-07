#ifndef IVW_TRANSFEREDITOR_H
#define IVW_TRANSFEREDITOR_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPointF>

#include<vector>

#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/ports/port.h>

#include <inviwo/core/ports/imageport.h>
#include <inviwo/qt/editor/transfereditorcontrolpoint.h>
#include <inviwo/qt/editor/transfereditorlineitem.h>

namespace inviwo {
    class TransferEditor : public QGraphicsScene {

    public :
        TransferEditor();
        TransferEditor(PropertyWidgetQt *parent_, TransferFunction* transferFunc_);
        ~TransferEditor();
        //float* getTransferValues();
        void sortPoints();
        void sortLines();

    protected :
        void mousePressEvent(QGraphicsSceneMouseEvent *e);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

        void addPoint(QGraphicsSceneMouseEvent *e);
        void removePoint(QGraphicsSceneMouseEvent *e);
        void calcTransferValues();

    private :
        static const std::string logSource_;
        std::vector<TransferEditorControlPoint*> points;
        std::vector<TransferEditorLineItem*> lines;
        ImageRAMfloat16* transferImage;
        TransferFunction* transferFunc;
        PropertyWidgetQt *parent;
    };

} // namespace

#endif // IVW_TRANSFEREDITOR_H