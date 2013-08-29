#ifndef IVW_TRANSFERFUNCTIONEDITORVIEW_H
#define IVW_TRANSFERFUNCTIONEDITORVIEW_H

//Inviwo includes
#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/qt/editor/transferfunctioneditor.h>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>

//Qt includes
#include <QtEvents>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace inviwo {
class IVW_QTWIDGETS_API TransferFunctionEditorView : public QGraphicsView {

	Q_OBJECT;

signals:
	void resized();

protected:
	void resizeEvent ( QResizeEvent * event );
};
} // namespace

#endif // IVW_TRANSFERFUNCTIONEDITORVIEW_H