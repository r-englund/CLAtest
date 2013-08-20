#ifndef IVW_TRANSFERFUNCTIONEDITORVIEW_H
#define IVW_TRANSFERFUNCTIONEDITORVIEW_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsView>
#include <inviwo/qt/editor/transferfunctioneditor.h>
#include <QGraphicsSceneResizeEvent>
#include <QtEvents>


namespace inviwo {
class IVW_QTWIDGETS_API TransferFunctionEditorView : public QGraphicsView, public VoidObservable{
	
signals:
	//void resized();

protected:
	void resizeEvent ( QResizeEvent * event ){
		//emit resized();
		static_cast<TransferFunctionEditor*>(scene())->repositionPoints();
		QGraphicsView::resizeEvent( event );
	}
};
} // namespace

#endif // IVW_TRANSFERFUNCTIONEDITORVIEW_H