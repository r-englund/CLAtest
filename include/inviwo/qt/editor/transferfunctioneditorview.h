#ifndef IVW_TRANSFERFUNCTIONEDITORVIEW_H
#define IVW_TRANSFERFUNCTIONEDITORVIEW_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsView>
#include <inviwo/qt/editor/transferfunctioneditor.h>
#include <QGraphicsSceneResizeEvent>
#include <QtEvents>


namespace inviwo {
class IVW_QTWIDGETS_API TransferFunctionEditorView : public QGraphicsView{
protected:
	void resizeEvent ( QResizeEvent * event ){
		std::cout << "resizeEvent" << std::endl;
		static_cast<TransferFunctionEditor*>(scene())->repositionPoints();
		QGraphicsView::resizeEvent( event );
	}
};
} // namespace

#endif // IVW_TRANSFERFUNCTIONEDITORVIEW_H