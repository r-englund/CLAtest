#include <inviwo/qt/widgets/properties/transferfunctioneditorcontrolpointgroup.h>

namespace inviwo {
    TransferFunctionEditorControlPointGroup::TransferFunctionEditorControlPointGroup(){
        std::cout << "hej" << std::endl;
    };

    TransferFunctionEditorControlPointGroup::~TransferFunctionEditorControlPointGroup(){};

    void TransferFunctionEditorControlPointGroup::paint(QPainter* painter, const QStyleOptionGraphicsItem* options, QWidget* widget) {
        IVW_UNUSED_PARAM(options);
        IVW_UNUSED_PARAM(widget);
    }

    QRectF TransferFunctionEditorControlPointGroup::boundingRect() const {
        float bboxsize = size_ + 10.0f;
        float left = pos().x() - bboxsize/2.0f;
        float top = pos().y() - bboxsize/2.0f;
        return QRectF(-bboxsize/2.0, -bboxsize/2.0f, bboxsize, bboxsize);
    }

    void TransferFunctionEditorControlPointGroup::mousePressEvent ( QGraphicsSceneMouseEvent *e ){}

    void TransferFunctionEditorControlPointGroup::mouseReleaseEvent( QGraphicsSceneMouseEvent *e ){}

    void TransferFunctionEditorControlPointGroup::mouseMoveEvent(QGraphicsSceneMouseEvent * e){}

    void TransferFunctionEditorControlPointGroup::addPoint(TransferFunctionEditorControlPoint* p){
    }
} // namespace