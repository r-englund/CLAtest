#include <QPainter>

#include <inviwo/qt/editor/transfereditorgraphicsitem.h>

namespace inviwo {

    TransferEditorGraphicsItem::TransferEditorGraphicsItem(QPointF startPoint):startPoint_(startPoint){ 
    }

	TransferEditorGraphicsItem::~TransferEditorGraphicsItem() {}

	void TransferEditorGraphicsItem::paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget) {
		IVW_UNUSED_PARAM(options);
		IVW_UNUSED_PARAM(widget);

        p->drawEllipse(this->startPoint_.x(), this->startPoint_.y(), 10, 10);		
	}

    QRectF TransferEditorGraphicsItem::boundingRect() const {
        return QRectF(startPoint_.x(), startPoint_.y(), 10, 10);
    }



} // namespace