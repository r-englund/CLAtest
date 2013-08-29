#include <inviwo/qt/editor/transferfunctioneditorview.h>

namespace inviwo{
	void TransferFunctionEditorView::resizeEvent ( QResizeEvent * event ){
		emit resized();
		static_cast<TransferFunctionEditor*>(scene())->repositionPoints();
		QGraphicsView::resizeEvent( event );
	}
}
